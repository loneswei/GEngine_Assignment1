#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"
#include "Character.h"
#include <random>

USING_NS_CC;

#define BACKGROUND_SCROLL_SPEED 7.f

#define WALL_MOVESPEED 250.0f
#define WALL_CONTENTSIZE_X 50.0f

#define SAMURAI_SPAWN_TIMING 10.f
#define COIN_SPAWN_TIMING 5.0f
#define MAGNET_SPAWN_TIMING 16.0f
#define SHIELD_SPAWN_TIMING 16.0f
#define SPIKE_SPAWN_TIMING 3.0f
#define SHURIKEN_SPAWN_TIMING 6.0f

#define COIN_SCORE 100.0f
#define FALL_SPEED 200.0f
#define MAGNET_STRENGTH 1050.0f

#define RESUME_POSITION 0.55
#define TUTORIAL2_POSITION 0.40
#define RESTART_POSITION 0.45
#define MAINMENU_POSITION 0.35

Scene* HelloWorld::createScene()
{
	// return scene
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

#ifdef SDKBOX_ENABLED
	if (!sdkbox::PluginFacebook::isLoggedIn())
	{
		sdkbox::PluginFacebook::login();
	}
#endif

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	playingSize = Size(visibleSize.width, visibleSize.height);

	// Init all game objects(walls,traps,items,enemies,player)
	GameObjectsInit();

	// Init all labels(text on screen)
	LabelInit();

	PauseUI();

	GameOverUI();

	tutorialbackground = Sprite::create("tutorial.png");
	tutorialbackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tutorialbackground->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
	tutorialbackground->setVisible(false);
	this->addChild(tutorialbackground, 1);

	backbutton = ui::Button::create("back.png");
	backbutton->setPosition(Vec2(playingSize.width * 0.9f, playingSize.height * 0.95));
	backbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//check if character is dead here
		tutormode = false;
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });
	backbutton->setVisible(false);
	this->addChild(backbutton, 1);

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/Bgm/menu.mp3", true);

	//audio->playBackgroundMusic("mymusic", false);
	movespeed = WALL_MOVESPEED;

	// Key Pressed movement
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Key Released movement
	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	// Mouse button Up
	auto listener3 = EventListenerMouse::create();
	listener3->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);

	// Touch detection for Jump
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Touch Listener for enemy
	auto enemytouch = EventListenerTouchOneByOne::create();
	enemytouch->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan2, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(enemytouch, this);


	//Init Last Spawned Objects
	LastSpawnedItem = nullptr;
	LastSpawnedTrap = nullptr;
	LastSpawnedEnemy = nullptr;

	// Call Update function
	this->scheduleUpdate();

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	//// Move to the right
	//if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	//{
	//	mainChar.MoveChar(eRight);
	//}
	//// Move to the left
	//if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	//{
	//	mainChar.MoveChar(eLeft);
	//}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	// Debug Code - Exit application
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}

	// Debug Code - Spawn Samurai Enemy
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		SpawnSamuraiEnemy();
	}

	// Debug Code - Spawn Coin
	if (keyCode == EventKeyboard::KeyCode::KEY_C)
	{
		SpawnCoin();
	}

	// Debug Code - Spawn Magnet
	if (keyCode == EventKeyboard::KeyCode::KEY_M)
	{
		SpawnMagnet();
	}

	// Jump to the opposite side
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		// Only allow Jumping when Status is eRun(a.k.a when player lands on wall)
		if (mainChar.getStatus() == mainChar.eRun)
		{
			mainChar.setStatus(mainChar.eJump);
			float LTarget = WALL_CONTENTSIZE_X * 1.2f;
			float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 1.2f);
			float height = mainChar.getSprite()->getPosition().y * 0.75f;

			mainChar.Jump(LTarget, RTarget, height);

			if (mainChar.getInvulActive() == true)
			{
				auto blink = CCBlink::create(3, 15);
				blink->initWithDuration(3, 15);
				mainChar.getSprite()->runAction(blink);
			}
		}
		if (enemyChar.getStatus() == enemyChar.eRun2)
		{
			enemyChar.setStatus(enemyChar.eJump2);
			float LTarget = WALL_CONTENTSIZE_X * 1.2f;
			float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 1.2f);
			float height = enemyChar.getSprite()->getPosition().y * 0.75f;

			enemyChar.Jump(LTarget, RTarget, height);

		}
	}
	//pause and resume here
	if (keyCode == EventKeyboard::KeyCode::KEY_P)
	{
		paused = true;
		//Director::sharedDirector()->pause();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_N)
	{
		Director::sharedDirector()->resume();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection2 == RESUME)
	{
		paused = false;
		Director::sharedDirector()->resume();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection2 == RESTART)
	{
		//create a new gamescene to restart.
		paused = false;
		Director::sharedDirector()->resume();
		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection2 == TUTORIAL2)
	{

		tutorialbackground->setVisible(true);

	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection2 == MAINMENU)
	{
		//go back to main menu
		Exit();
		paused = false;
		Director::sharedDirector()->resume();
		auto scene = MainMenu::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && mainChar.getAliveorNot() == true)
	{
		//go back to main menu

		Exit();
		auto scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		mainChar.setAliveorNot(false);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE && paused == true)
	{
		//go back to main menu
		tutorialbackground->setVisible(false);
		paused = false;
		Director::sharedDirector()->resume();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		//move down here
		switch (selection2)
		{
		case RESUME:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * RESTART_POSITION));
			selection2 = RESTART;
			break;
		case RESTART:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * TUTORIAL2_POSITION));
			selection2 = TUTORIAL2;
			break;
		case TUTORIAL2:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * MAINMENU_POSITION));
			selection2 = MAINMENU;
			break;
		case MAINMENU:
			break;
		}

	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		//move up here & check enum
		switch (selection2)
		{
		case RESUME:
			break;
		case RESTART:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * RESUME_POSITION));
			selection2 = RESUME;
			break;
		case TUTORIAL2:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * RESTART_POSITION));
			selection2 = RESTART;
			break;
		case MAINMENU:
			arrow2->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * TUTORIAL2_POSITION));
			selection2 = TUTORIAL2;
			break;
		}
	}
}

void HelloWorld::onMouseUp(Event * event)
{
	//EventMouse* e = (EventMouse*)event;
	//float x = e->getCursorX();
	//float y = e->getCursorY();

	//mainChar.MoveCharByCoord(x, y);
}

// Touch trigger jump
bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	Rect box = enemyChar.getSprite()->getBoundingBox();
	if (!box.containsPoint(touch->getLocation()))
	{
		if (mainChar.getStatus() == mainChar.eRun && enemyChar.getStatus() == enemyChar.eRun2)
		{
			mainChar.setStatus(mainChar.eJump);
			float LTarget = WALL_CONTENTSIZE_X * 1.2f;
			float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 1.2f);
			float height = mainChar.getSprite()->getPosition().y * 0.75f;

			mainChar.Jump(LTarget, RTarget, height);
			if (mainChar.getInvulActive())
			{
				auto blink = CCBlink::create(3, 15);
				blink->initWithDuration(3, 15);
				mainChar.getSprite()->runAction(blink);
			}

			enemyChar.setStatus(enemyChar.eJump2);
			float LeTarget = WALL_CONTENTSIZE_X * 1.2f;
			float ReTarget = playingSize.width - (WALL_CONTENTSIZE_X * 1.2f);
			float heeight = enemyChar.getSprite()->getPosition().y * 0.75f;

			enemyChar.Jump(LeTarget, ReTarget, heeight);
			return true;
		}
	}
	return false;
}

//for enemy 
bool HelloWorld::onTouchBegan2(Touch * touch, Event * event)
{
	Vec2 TouchPoint = touch->getLocation();
	Rect box = enemyChar.getSprite()->getBoundingBox();

	if (box.containsPoint(TouchPoint))
	{
		enemyChar.setAliveorNot(true);
		return true;
	}
	else
		return false;
}

void HelloWorld::update(float delta)
{
	LabelUpdate();

	// ---------- Auto Spawning ---------- 
	AutoSpawner(delta, samuraiSpawnTimer, SAMURAI_SPAWN_TIMING, eSamuraiEnemy);
	AutoSpawner(delta, coinSpawnTimer, COIN_SPAWN_TIMING, eCoin);
	AutoSpawner(delta, magnetSpawnTimer, MAGNET_SPAWN_TIMING, eMagnet);
	AutoSpawner(delta, shieldSpawnTimer, SHIELD_SPAWN_TIMING, eShield);
	AutoSpawner(delta, spikeSpawnTimer, SPIKE_SPAWN_TIMING, eSpike);
	AutoSpawner(delta, shurikenSpawnTimer, SHURIKEN_SPAWN_TIMING, eShuriken);
	// ---------- Auto Spawning ---------- 

	if (enemyChar.getAliveorNot() == false)
	{
		enemyChar.getSprite()->setVisible(true);
		if (movespeed > 0)
			movespeed -= 5 * delta;
	}
	else
	{
		movespeed = WALL_MOVESPEED;
		enemyChar.getSprite()->setVisible(false);
	}


	if (enemyChar.getAliveorNot() == true)
	{
		int random_directnum = RandomHelper::random_int(0, 300);

		if (random_directnum >= 299)
		{
			enemyChar.setAliveorNot(false);
		}
		else
		{
			enemyChar.setAliveorNot(true);
		}
	}
	// DEAD - GAME OVER
	if (mainChar.getLifeCount() <= 0 || movespeed <= 0)
	{
		mainChar.getSprite()->pause();
		mainChar.setAliveorNot(true);

		mainChar.getSprite()->setPositionY(mainChar.getSprite()->getPositionY() - FALL_SPEED * delta);
		const float spriteGameWidth = mainChar.getSprite()->getContentSize().width * mainChar.getSprite()->getScaleX() * 0.5f;
		if (mainChar.getSprite()->getPositionY() + spriteGameWidth < 0)
			mainChar.getSprite()->setVisible(false);

		enemyChar.getSprite()->setPositionY(enemyChar.getSprite()->getPositionY() - FALL_SPEED * delta);
		const float spriteGameWidth2 = enemyChar.getSprite()->getContentSize().width * enemyChar.getSprite()->getScaleX() * 0.5f;
		if (enemyChar.getSprite()->getPositionY() + spriteGameWidth2 < 0)
			enemyChar.getSprite()->setVisible(false);
	}


	if (paused)
	{
		resumebutton->setVisible(true);
		retrybutton->setVisible(true);
		mainmenubutton->setVisible(true);
		tutorialbutton->setVisible(true);
		pausebackground->setVisible(true);
		Director::sharedDirector()->pause();
		//scoreLabel2->setVisible(true);

	}
	else
	{
		resumebutton->setVisible(false);
		retrybutton->setVisible(false);
		mainmenubutton->setVisible(false);
		tutorialbutton->setVisible(false);
		pausebackground->setVisible(false);

		//		scoreLabel2->setVisible(false);
	}
	if (mainChar.getAliveorNot() == true)
	{
		deadLabel->setVisible(true);
		gameoverrestartbutton->setVisible(true);
		gameoverbackbutton->setVisible(true);
		//	scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
		instructiongameover->setVisible(true);
		gameoverbackground->setVisible(true);
		//Director::sharedDirector()->pause();

		const std::string Score = std::to_string((int)mainChar.getScore());
		int GoldEarned;

		//Less than 2 digits
		if (Score.size() < 2)
		{
			GoldEarned = 0;
		}
		else
		{
			GoldEarned = std::stoi(Score.substr(0, Score.size() - 2));
		}

		GoldEarnedLabel->setString("Gold Earned: " + std::to_string(GoldEarned));
		GoldEarnedLabel->setVisible(true);
	}
	else
	{
		//	scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, scoreLabel->getContentSize().height + distanceLabel->getContentSize().height));
		deadLabel->setVisible(false);
		instructiongameover->setVisible(false);
		gameoverbackground->setVisible(false);
		gameoverrestartbutton->setVisible(false);
		gameoverbackbutton->setVisible(false);
	}

	if (enemyChar.getAliveorNot() == true)
	{
		enemyChar.getSprite()->setVisible(false);
	}


	if (tutormode)
	{
		Director::sharedDirector()->pause();
		tutorialbackground->setVisible(true);
		backbutton->setVisible(true);
	}
	else
	{
		tutorialbackground->setVisible(false);
		backbutton->setVisible(false);
	}
	// Update Character
	mainChar.Update(delta);

	// Update Wall
	WallUpdate(delta);

	// Update Traps
	TrapUpdate(delta);

	// Update Items
	ItemUpdate(delta);

	// Update Enemies
	EnemyUpdate(delta);
	enemyChar.Update(delta);

	//Background Scrolling
	auto theBackgroundImage = BackgroundNode->getChildByName("BackgroundSprite");
	static auto DistanceBetweenPositions = (theBackgroundImage->getContentSize().height * theBackgroundImage->getScaleY() * 0.5f) - playingSize.height * 0.5f;

	if (playingSize.height * 0.5f - theBackgroundImage->getPositionY() <= DistanceBetweenPositions)
	{
		theBackgroundImage->setPositionY(theBackgroundImage->getPositionY() - delta * BACKGROUND_SCROLL_SPEED);
	}
}

float HelloWorld::RandomFloatRange(const float &a, const float &b)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(a, b);

	return dis(gen);
}

Enemy* HelloWorld::SpawnSamuraiEnemy()
{
	Enemy* Enemy = FetchEnemyObject(Enemy::ENEMY_SAMURAI);
	Enemy->setIsActive(true);
	Enemy->getEnemySprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	return Enemy;
}

ItemObject* HelloWorld::SpawnCoin()
{
	// Random choose to spawn at left side or right side
	int random_dir = RandomHelper::random_int(0, 9);
	// Random choose to spawn how many coins at one go
	int random_spawnAmount = RandomHelper::random_int(1, 5);
	// For recording position of previous coin so can spawn in a string
	float prevCoinPosY = 0.0f;

	for (random_spawnAmount; random_spawnAmount > 0; --random_spawnAmount)
	{
		ItemObject* Coin = FetchItemObject(ItemObject::ITEM_COIN);
		Coin->setItemSprite(Sprite::create("coin.png"));
		Coin->SpriteWidth = Coin->getItemSprite()->getContentSize().width * Coin->getItemSprite()->getScaleX();
		Coin->halfSpriteWidth = Coin->SpriteWidth * 0.5f;
		Coin->setIsActive(true);
		Coin->getItemSprite()->resume();
		Coin->CoinBeingCollected = false;
		if (!Coin->getItemSprite()->isVisible())
			Coin->getItemSprite()->setVisible(true);
		Coin->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		Coin->setCoinScore(COIN_SCORE);

		if (random_dir >= 5)
		{
			Coin->setItemDirection(ItemObject::ITEM_RIGHT);
		}
		else
		{
			Coin->setItemDirection(ItemObject::ITEM_RIGHT);
		}

		// Set position according to Item direction
		switch (Coin->getItemDirection())
		{
		case ItemObject::ITEM_RIGHT:
			if (prevCoinPosY == 0.0f)
			{
				Coin->getItemSprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height));
			}
			else
			{
				Coin->getItemSprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), prevCoinPosY + Coin->getItemSprite()->getContentSize().height * 2));
			}
			Coin->getItemSprite()->setRotation(-90);
			prevCoinPosY = Coin->getItemSprite()->getPositionY();
			break;
		case ItemObject::ITEM_LEFT:
			if (prevCoinPosY == 0.0f)
			{
				Coin->getItemSprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height));
			}
			else
			{
				Coin->getItemSprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, prevCoinPosY + Coin->getItemSprite()->getContentSize().height * 2));
			}
			Coin->getItemSprite()->setRotation(90);
			prevCoinPosY = Coin->getItemSprite()->getPositionY();
			break;
		}
		itemObjects->addChild(Coin->getItemSprite());

		//Return the last coin spawned
		if (random_spawnAmount - 1 == 0)
		{
			return Coin;
		}
	}
}

ItemObject* HelloWorld::SpawnMagnet()
{
	ItemObject* Magnet = FetchItemObject(ItemObject::ITEM_MAGNET);
	Magnet->setItemSprite(Sprite::create("magnet.png"));
	Magnet->SpriteWidth = Magnet->getItemSprite()->getContentSize().width * Magnet->getItemSprite()->getScaleX();
	Magnet->halfSpriteWidth = Magnet->SpriteWidth * 0.5f;
	Magnet->setIsActive(true);
	Magnet->getItemSprite()->resume();
	if (!Magnet->getItemSprite()->isVisible())
		Magnet->getItemSprite()->setVisible(true);
	Magnet->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	itemObjects->addChild(Magnet->getItemSprite());

	return Magnet;
}

ItemObject* HelloWorld::SpawnShield()
{
	ItemObject* Shield = FetchItemObject(ItemObject::ITEM_SHIELD);
	Shield->setItemSprite(Sprite::create("shield.png"));
	Shield->SpriteWidth = Shield->getItemSprite()->getContentSize().width * Shield->getItemSprite()->getScaleX();
	Shield->halfSpriteWidth = Shield->SpriteWidth * 0.5f;
	Shield->setIsActive(true);
	Shield->getItemSprite()->resume();
	if (!Shield->getItemSprite()->isVisible())
		Shield->getItemSprite()->setVisible(true);
	Shield->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	itemObjects->addChild(Shield->getItemSprite());

	return Shield;
}

TrapObject* HelloWorld::SpawnSpike()
{
	TrapObject* Spike = FetchTrapObject(TrapObject::TRAP_SPIKES);
	Spike->setTrapSprite(Sprite::create("spiketrap.png"));
	Spike->SpriteWidth = Spike->getTrapSprite()->getContentSize().width * Spike->getTrapSprite()->getScaleX();
	Spike->halfSpriteWidth = Spike->SpriteWidth * 0.5f;
	Spike->setIsActive(true);
	Spike->getTrapSprite()->resume();
	if (!Spike->getTrapSprite()->isVisible())
		Spike->getTrapSprite()->setVisible(true);
	Spike->getTrapSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	trapObjects->addChild(Spike->getTrapSprite());

	return Spike;
}

TrapObject* HelloWorld::SpawnShuriken()
{
	TrapObject* Shuriken = FetchTrapObject(TrapObject::TRAP_SHURIKEN);
	Shuriken->setTrapSprite(Sprite::create("shuriken.png"));
	Shuriken->SpriteWidth = Shuriken->getTrapSprite()->getContentSize().width * Shuriken->getTrapSprite()->getScaleX();
	Shuriken->halfSpriteWidth = Shuriken->SpriteWidth * 0.5f;
	Shuriken->setIsActive(true);
	Shuriken->getTrapSprite()->resume();
	if (!Shuriken->getTrapSprite()->isVisible())
		Shuriken->getTrapSprite()->setVisible(true);
	Shuriken->getTrapSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	trapObjects->addChild(Shuriken->getTrapSprite());

	return Shuriken;
}

void HelloWorld::GameObjectsInit()
{
	//Backgground Image
	BackgroundNode = Node::create();
	BackgroundNode->setName("BackgroundNode");
	auto BackgroundSprite = Sprite::create("GameBackground.jpg");
	BackgroundSprite->setName("BackgroundSprite");

	auto DistanceBewtweenPos = (BackgroundSprite->getContentSize().height * 0.5f * BackgroundSprite->getScaleY()) - (playingSize.height * 0.5f);
	BackgroundSprite->setPosition(playingSize.width * 0.5f, (playingSize.height * 0.5f) + DistanceBewtweenPos);
	BackgroundNode->addChild(BackgroundSprite, 1);
	this->addChild(BackgroundNode, 0);

	// ------------------ Init Walls ----------------------
	wallObjects = Node::create();
	wallObjects->setName("WallObjects");

	auto wallSprite_LEFT = Sprite::create("woodwall.jpg");
	auto wallSprite_LEFT02 = Sprite::createWithTexture(wallSprite_LEFT->getTexture());
	auto wallSprite_RIGHT = Sprite::createWithTexture(wallSprite_LEFT->getTexture());
	auto wallSprite_RIGHT02 = Sprite::createWithTexture(wallSprite_LEFT->getTexture());

	wallSprite_LEFT->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height));
	wallSprite_LEFT02->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height + playingSize.height * 0.25f));
	wallSprite_RIGHT->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height));
	wallSprite_RIGHT02->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height + playingSize.height * 0.25f));

	wallSprite_LEFT->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	wallSprite_LEFT->setPosition(Vec2(0.f, playingSize.height * 0.5f));

	wallSprite_LEFT02->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	wallSprite_LEFT02->setPosition(Vec2(0, (playingSize.height * 0.5f) + playingSize.height));

	wallSprite_RIGHT->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	wallSprite_RIGHT->setPosition(Vec2(playingSize.width, playingSize.height * 0.5f));

	wallSprite_RIGHT02->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	wallSprite_RIGHT02->setPosition(Vec2(playingSize.width, (playingSize.height * 0.5f) + playingSize.height));

	wallObjects->addChild(wallSprite_LEFT, 0);
	wallObjects->addChild(wallSprite_LEFT02, 0);
	wallObjects->addChild(wallSprite_RIGHT, 0);
	wallObjects->addChild(wallSprite_RIGHT02, 0);
	this->addChild(wallObjects, 1);
	// ------------------ Init Walls ----------------------

	// ------------------ Init Traps ----------------------
	trapObjects = Node::create();
	trapObjects->setName("TrapObjects");
	this->addChild(trapObjects);
	spikeSpawnTimer = RandomFloatRange(0, SPIKE_SPAWN_TIMING);
	shurikenSpawnTimer = RandomFloatRange(0, SHURIKEN_SPAWN_TIMING);
	// ------------------ Init Traps ----------------------

	// ------------------ Init Items ----------------------
	itemObjects = Node::create();
	itemObjects->setName("ItemObjects");
	this->addChild(itemObjects);
	coinSpawnTimer = RandomFloatRange(0, COIN_SPAWN_TIMING);
	magnetSpawnTimer = RandomFloatRange(0, MAGNET_SPAWN_TIMING);
	shieldSpawnTimer = RandomFloatRange(0, SHIELD_SPAWN_TIMING);
	// ------------------ Init Items ----------------------

	// ------------------ Init Enemy ----------------------
	enemyObjects = Node::create();
	enemyObjects->setName("EnemyObjects");
	this->addChild(enemyObjects);
	samuraiSpawnTimer = 0.0f;

	auto enemyobject = Node::create();
	enemyobject->setName("enemyobject");
	auto enemysprite = Sprite::create("samurai_run_right_01.png");
	enemyChar.init("samurai_run_right_01.png", Vec2::ANCHOR_MIDDLE, (playingSize.width - (WALL_CONTENTSIZE_X * 1.2f)), (enemysprite->getContentSize().width * 2), "enemy");
	enemyobject->addChild(enemyChar.getSprite(), 1);
	this->addChild(enemyobject, 1);
	enemyspritewidth = enemyChar.getSprite()->getContentSize().width * enemyChar.getSprite()->getScaleX() * 0.5f;
	enemyspawntimer = 0.0f;
	// ------------------ Init Enemy ----------------------

	// ------------------ Init Player ----------------------
	auto playerObject = Node::create();
	playerObject->setName("PlayerObject");
	auto playerSprite = Sprite::create("Skins/Default/run_right_01.png");
	mainChar.init("Skins/Default/run_right_01.png", Vec2::ANCHOR_MIDDLE, (playingSize.width - (WALL_CONTENTSIZE_X * 1.2f)), (playerSprite->getContentSize().width * 2), "Player");
	playerObject->addChild(mainChar.getSprite(), 1);
	this->addChild(playerObject, 1);
	characterSpriteWidth = mainChar.getSprite()->getContentSize().width * mainChar.getSprite()->getScaleX() * 0.5f;

	// Player's Shield
	auto playerShieldObject = Node::create();
	playerShieldObject->setName("PlayerShieldObject");
	playerShieldObject->addChild(mainChar.getShieldSprite(), 1);
	this->addChild(playerShieldObject, 1);
	// ------------------ Init Player ----------------------
}

void HelloWorld::LabelInit()
{
	// Distance Travelled
	distanceLabel = Label::createWithTTF("Distance Travelled: " + std::to_string((int)mainChar.getDistanceTravelled()), "fonts/Marker Felt.ttf", 24);
	distanceLabel->setPosition(Vec2(playingSize.width * 0.5f, distanceLabel->getContentSize().height));
	this->addChild(distanceLabel, 1);

	// Score
	scoreLabel = Label::createWithTTF("Score: " + std::to_string((int)mainChar.getScore()), "fonts/Marker Felt.ttf", 24);
	scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, scoreLabel->getContentSize().height + distanceLabel->getContentSize().height));
	this->addChild(scoreLabel, 1);

	// Life Count
	lifeLabel = Label::createWithTTF("Life: " + std::to_string(mainChar.getLifeCount()), "fonts/Marker Felt.ttf", 24);
	lifeLabel->setPosition(Vec2(playingSize.width * 0.5f, lifeLabel->getContentSize().height + distanceLabel->getContentSize().height + scoreLabel->getContentSize().height));
	this->addChild(lifeLabel, 1);

	// Shield Duration
	shieldLabel = Label::createWithTTF("Shield: " + std::to_string((int)(mainChar.getShieldDuration() - mainChar.getShieldTimer())), "fonts/Marker Felt.ttf", 24);
	shieldLabel->setPosition(Vec2(playingSize.width * 0.5f, shieldLabel->getContentSize().height + distanceLabel->getContentSize().height + scoreLabel->getContentSize().height + lifeLabel->getContentSize().height));
	this->addChild(shieldLabel, 1);

	// Magnet Duration
	magnetLabel = Label::createWithTTF("Magnet: " + std::to_string((int)(mainChar.getMagnetDuration() - mainChar.getMagnetTimer())), "fonts/Marker Felt.ttf", 24);
	magnetLabel->setPosition(Vec2(playingSize.width * 0.5f, magnetLabel->getContentSize().height + distanceLabel->getContentSize().height + scoreLabel->getContentSize().height + lifeLabel->getContentSize().height + shieldLabel->getContentSize().height));
	this->addChild(magnetLabel, 1);

	// Invulnability Duration
	invulLabel = Label::createWithTTF("Invul: " + std::to_string((int)(mainChar.getInvulDuration() - mainChar.getInvulTimer())), "fonts/Marker Felt.ttf", 24);
	invulLabel->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height - invulLabel->getContentSize().height));
	this->addChild(invulLabel, 1);
}

void HelloWorld::LabelUpdate()
{
	// Score
	scoreLabel->setString("Score: " + std::to_string((int)mainChar.getScore()));

	// Distance Travelled
	distanceLabel->setString("Distance Travelled: " + std::to_string((int)mainChar.getDistanceTravelled()));

	// Life Count
	lifeLabel->setString("Life: " + std::to_string(mainChar.getLifeCount()));

	// Shield Duration
	if (mainChar.getShieldActive())
		shieldLabel->setString("Shield Left: " + std::to_string((int)(mainChar.getShieldDuration() - mainChar.getShieldTimer())));
	else
		shieldLabel->setString("");

	// Magnet Duration
	if (mainChar.getMagnetActive())
	{
		magnetLabel->setString("Magnet Left: " + std::to_string((int)(mainChar.getMagnetDuration() - mainChar.getMagnetTimer())));
		if (!mainChar.getShieldActive())
			magnetLabel->setPosition(Vec2(magnetLabel->getPositionX(), magnetLabel->getPositionY() - shieldLabel->getContentSize().height));
		else
			magnetLabel->setPosition(Vec2(playingSize.width * 0.5f, magnetLabel->getContentSize().height + distanceLabel->getContentSize().height + scoreLabel->getContentSize().height + lifeLabel->getContentSize().height + shieldLabel->getContentSize().height));
	}
	else
		magnetLabel->setString("");

	// Invulnability Duration
	if (mainChar.getInvulActive())
		invulLabel->setString("invul Left: " + std::to_string((int)(mainChar.getInvulDuration() - mainChar.getInvulTimer())));
	else
		invulLabel->setString("");
}

void HelloWorld::AutoSpawner(float delta, float &timer, float timing, ESpawner _eSpawn)
{
	timer += 1 * delta;

	if (timer >= timing)
	{
		int random_dir = RandomHelper::random_int(0, 9);

		bool isSpawnRight;
		Vec2 SpawnPos;

		// Call related Spawn function according to passed in spawner enum
		switch (_eSpawn)
		{
		case eSamuraiEnemy:
		{
			// Random choose to spawn at left side or right side
			if (random_dir >= 5)
			{
				isSpawnRight = true;
				SpawnPos = Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 1.2f)), playingSize.height);
			}
			else
			{
				isSpawnRight = false;
				SpawnPos = Vec2(WALL_CONTENTSIZE_X * 1.2f, playingSize.height);
			}

			//Check against last spawned item
			if (LastSpawnedItem)
			{
				//Spawned within another object
				if (LastSpawnedItem->getItemSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedItem->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned trap
			if (LastSpawnedTrap)
			{
				//Spawned within another object
				if (LastSpawnedTrap->getTrapSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedTrap->SpriteWidth ||
					SpawnPos.y - LastSpawnedTrap->getTrapSprite()->getPositionY() < LastSpawnedTrap->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned enemy
			if (LastSpawnedEnemy)
			{
				//Spawned within another object
				if (LastSpawnedEnemy->getEnemySprite()->getPosition().getDistance(SpawnPos) < LastSpawnedEnemy->SpriteWidth ||
					SpawnPos.y - LastSpawnedEnemy->getEnemySprite()->getPositionY() < LastSpawnedEnemy->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Spawn the object
			Enemy* theEnemy = SpawnSamuraiEnemy();
			theEnemy->getEnemySprite()->setPosition(SpawnPos);
			theEnemy->setEnemyDirection((Enemy::ENEMY_DIRECTION)isSpawnRight);

			//Becomes the last spawned
			LastSpawnedEnemy = theEnemy;

			// Call Run animating Function
			theEnemy->Run();

			break;
		}
		case eCoin:
			////Because of the way coins are spawned, the coins will not check if its in spawning in a valid pos
			////Intead others will be the one checking against coins
			LastSpawnedItem = SpawnCoin();
			break;
		case eMagnet:
		{
			// Random choose to spawn at left side or right side
			if (random_dir >= 5)
			{
				isSpawnRight = true;
				SpawnPos = Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height);
			}
			else
			{
				isSpawnRight = false;
				SpawnPos = Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height);
			}

			//Check against last spawned item
			if (LastSpawnedItem)
			{
				//Spawned within another object
				if (LastSpawnedItem->getItemSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedItem->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned trap
			if (LastSpawnedTrap)
			{
				//Spawned within another object
				if (LastSpawnedTrap->getTrapSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedTrap->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned enemy
			if (LastSpawnedEnemy)
			{
				//Spawned within another object
				if (LastSpawnedEnemy->getEnemySprite()->getPosition().getDistance(SpawnPos) < LastSpawnedEnemy->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Spawn the object
			ItemObject* theMagnet = SpawnMagnet();
			theMagnet->getItemSprite()->setPosition(SpawnPos);
			theMagnet->setItemDirection((ItemObject::ITEM_DIRECTION)isSpawnRight);

			if (isSpawnRight)
			{
				theMagnet->getItemSprite()->setRotation(-90);
			}
			else
			{
				theMagnet->getItemSprite()->setRotation(90);
			}

			//Becomes the last spawned
			LastSpawnedItem = theMagnet;
			break;
		}
		case eShield:
		{
			if (random_dir >= 5)
			{
				isSpawnRight = true;
				SpawnPos = Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height);
			}
			else
			{
				isSpawnRight = false;
				SpawnPos = Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height);
			}

			//Check against last spawned item
			if (LastSpawnedItem)
			{
				//Spawned within another object
				if (LastSpawnedItem->getItemSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedItem->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned trap
			if (LastSpawnedTrap)
			{
				//Spawned within another object
				if (LastSpawnedTrap->getTrapSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedTrap->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned enemy
			if (LastSpawnedEnemy)
			{
				//Spawned within another object
				if (LastSpawnedEnemy->getEnemySprite()->getPosition().getDistance(SpawnPos) < LastSpawnedEnemy->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Spawn the object
			ItemObject* theShield = SpawnShield();;
			theShield->getItemSprite()->setPosition(SpawnPos);
			theShield->setItemDirection((ItemObject::ITEM_DIRECTION)isSpawnRight);

			if (isSpawnRight)
			{
				theShield->getItemSprite()->setRotation(-90);
			}
			else
			{
				theShield->getItemSprite()->setRotation(90);
			}

			//Becomes the last spawned
			LastSpawnedItem = theShield;

			break;
		}
		case eSpike:
		{
			if (random_dir >= 5)
			{
				isSpawnRight = true;
				SpawnPos = Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height);
			}
			else
			{
				isSpawnRight = false;
				SpawnPos = Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height);
			}

			//Check against last spawned item
			if (LastSpawnedItem)
			{
				//Spawned within another object
				if (LastSpawnedItem->getItemSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedItem->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned trap
			if (LastSpawnedTrap)
			{
				//Spawned within another object
				if (LastSpawnedTrap->getTrapSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedTrap->SpriteWidth ||
					SpawnPos.y - LastSpawnedTrap->getTrapSprite()->getPositionY() < LastSpawnedTrap->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned enemy
			if (LastSpawnedEnemy)
			{
				//Spawned within another object
				if (LastSpawnedEnemy->getEnemySprite()->getPosition().getDistance(SpawnPos) < LastSpawnedEnemy->SpriteWidth ||
					SpawnPos.y - LastSpawnedEnemy->getEnemySprite()->getPositionY() < LastSpawnedEnemy->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Spawn the object
			TrapObject *theSpike = SpawnSpike();
			theSpike->getTrapSprite()->setPosition(SpawnPos);
			theSpike->setTrapDirection((TrapObject::TRAP_DIRECTION)isSpawnRight);

			if (isSpawnRight)
			{
				theSpike->getTrapSprite()->setRotation(-90);
			}
			else
			{
				theSpike->getTrapSprite()->setRotation(90);
			}

			//Becomes the last spawned
			LastSpawnedTrap = theSpike;

			break;
		}
		case eShuriken:
		{
			// Random choose to spawn at left side or right side
			if (random_dir >= 5)
			{
				isSpawnRight = true;
				SpawnPos = Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.5f)), playingSize.height);
			}
			else
			{
				isSpawnRight = false;
				SpawnPos = Vec2(WALL_CONTENTSIZE_X * 0.5f, playingSize.height);
			}

			//Check against last spawned item
			if (LastSpawnedItem)
			{
				//Spawned within another object
				if (LastSpawnedItem->getItemSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedItem->SpriteWidth)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned trap
			if (LastSpawnedTrap)
			{
				//Spawned within another object
				if (LastSpawnedTrap->getTrapSprite()->getPosition().getDistance(SpawnPos) < LastSpawnedTrap->SpriteWidth ||
					SpawnPos.y - LastSpawnedTrap->getTrapSprite()->getPositionY() < LastSpawnedTrap->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Check against last spawned enemy
			if (LastSpawnedEnemy)
			{
				//Spawned within another object
				if (LastSpawnedEnemy->getEnemySprite()->getPosition().getDistance(SpawnPos) < LastSpawnedEnemy->SpriteWidth ||
					SpawnPos.y - LastSpawnedEnemy->getEnemySprite()->getPositionY() < LastSpawnedEnemy->SpriteWidth
					)
				{
					//Delay spawning by random amount
					timer = timing - RandomFloatRange(1.f, 3.f);
					return;
				}
			}

			//Spawn the object
			TrapObject *theShuriken = SpawnShuriken();
			theShuriken->getTrapSprite()->setPosition(SpawnPos);
			theShuriken->setTrapDirection((TrapObject::TRAP_DIRECTION)isSpawnRight);

			if (isSpawnRight)
			{
				theShuriken->getTrapSprite()->setRotation(-90);
			}
			else
			{
				theShuriken->getTrapSprite()->setRotation(90);
			}

			//Becomes the last spawned
			LastSpawnedTrap = theShuriken;

			break;
		}
		}
		timer = 0.0f;
	}
}

void HelloWorld::WallUpdate(float delta)
{
	//Get the array of walls from wallObjects node
	auto wallObjectsArray = wallObjects->getChildren();

	//Move each wall sprite downwards
	for (auto wallSprite : wallObjectsArray)
	{
		if (wallSprite->getPositionY() < -playingSize.height * 0.5f)
		{
			wallSprite->setPositionY((playingSize.height * 0.5f) + playingSize.height);
		}
		else
		{
			wallSprite->setPositionY(wallSprite->getPositionY() - movespeed * delta);
		}
	}
}

void HelloWorld::TrapUpdate(float delta)
{
	//Update each trap in trap list & collision check
	for (auto trapObj : trapObjectList)
	{
		if (!trapObj->getIsActive())
		{
			continue;
		}

		//Update trapa
		trapObj->getTrapSprite()->setPositionY(trapObj->getTrapSprite()->getPositionY() - movespeed * delta);

		const float spriteGameWidth = (trapObj->getTrapSprite()->getContentSize().width) * (trapObj->getTrapSprite()->getScaleX()) * 0.5f;

		//Disable trap if it goes below the screen
		if (trapObj->getTrapSprite()->getPositionY() + spriteGameWidth < 0)
		{
			trapObj->setIsActive(false);
			trapObj->getTrapSprite()->pause();
			trapObj->getTrapSprite()->setVisible(false);
		}
		//Collision
		if (mainChar.getLifeCount() > 0)
		{
			if ((trapObj->getTrapSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
			{
				//Lose a life here
				if (!mainChar.getShieldActive() && !mainChar.getInvulActive())
				{
					mainChar.setLifeCount(mainChar.getLifeCount() - 1);
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->playEffect("Audio/SoundEffect/HeroDeath.mp3", false, 1.0f, 0.0f, 1.0f);
					mainChar.setInvulActive(true);

					if (mainChar.getAliveorNot() == false)
					{
						auto blink = CCBlink::create(3, 15);
						blink->initWithDuration(3, 15);
						mainChar.getSprite()->runAction(blink);
					}
					else
						mainChar.Run();

				}
			}
		}
	}
}

void HelloWorld::ItemUpdate(float delta)
{
	static auto AudioManager = CocosDenshion::SimpleAudioEngine::getInstance();

	//Update each item in item list & collision check
	for (auto itemObj : itemObjectList)
	{
		if (!itemObj->getIsActive())
		{
			continue;
		}
		if (itemObj->getItemType() != ItemObject::ITEM_COIN)
		{
			//Update position of item
			itemObj->getItemSprite()->setPositionY(itemObj->getItemSprite()->getPositionY() - movespeed * delta);
		}
		//Update item


		// Update Coin
		if (itemObj->getItemType() == ItemObject::ITEM_COIN)
		{
			if (itemObj->CoinBeingCollected)
			{
				Vec2 dir = (mainChar.getSprite()->getPosition() - itemObj->getItemSprite()->getPosition()).getNormalized();
				dir *= MAGNET_STRENGTH * delta;

				itemObj->getItemSprite()->setPosition(itemObj->getItemSprite()->getPosition() + dir);
			}
			else if (mainChar.getLifeCount() > 0 && mainChar.getMagnetActive())
			{
				// Magnet sucks coins to character
				if (itemObj->getItemSprite()->isVisible() &&
					(itemObj->getItemSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= MAGNET_STRENGTH)
				{
					itemObj->CoinBeingCollected = true;
				}
			}

			itemObj->getItemSprite()->setPositionY(itemObj->getItemSprite()->getPositionY() - movespeed * delta);
		}
		const float spriteGameWidth = itemObj->getItemSprite()->getContentSize().width * itemObj->getItemSprite()->getScaleX() * 0.5f;

		//Disable item if it goes below the screen
		if (itemObj->getItemSprite()->getPositionY() + spriteGameWidth < 0)
		{
			itemObj->setIsActive(false);
			itemObj->getItemSprite()->pause();
			itemObj->getItemSprite()->setVisible(false);
		}
		//Player Collision
		if (mainChar.getLifeCount() > 0)
		{
			if ((itemObj->getItemSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
			{
				switch (itemObj->getItemType())
				{
				case ItemObject::ITEM_COIN:
				{
					mainChar.setScore(mainChar.getScore() + itemObj->getCoinScore());
					AudioManager->playEffect("Audio/SoundEffect/coin_pickup.wav");
					break;
				}
				case ItemObject::ITEM_SHIELD:
				{
					if (!mainChar.getShieldActive())
					{
						mainChar.setShieldActive(true);
						mainChar.getShieldSprite()->setVisible(true);

						AudioManager->playEffect("Audio/SoundEffect/shield_pickup.mp3", false, 1, 0, 10);
					}
					else
					{
						mainChar.setShieldTimer(0.0f);
						AudioManager->playEffect("Audio/SoundEffect/shield_pickup.mp3", false, 1, 0, 10);
					}
					break;
				}
				case ItemObject::ITEM_MAGNET:
				{
					if (!mainChar.getMagnetActive())
					{
						mainChar.setMagnetActive(true);
						AudioManager->playEffect("Audio/SoundEffect/magnet_pickup.mp3", false, 1, 0, 0.1f);
					}
					else
					{
						mainChar.setMagnetTimer(0.0f);
						AudioManager->playEffect("Audio/SoundEffect/magnet_pickup.mp3", false, 1, 0, 0.1f);
					}
					break;
				}
				}
				itemObj->setIsActive(false);
				itemObj->getItemSprite()->pause();
				itemObj->getItemSprite()->setVisible(false);
			}
		}
	}
}

void HelloWorld::EnemyUpdate(float delta)
{
	// Update each enemy in enemy list
	for (auto enemy : enemyObjectList)
	{
		if (!enemy->getIsActive())
		{
			continue;
		}
		//Update enemy
		enemy->getEnemySprite()->setPositionY(enemy->getEnemySprite()->getPositionY() - movespeed * delta);

		const float spriteGameWidth = enemy->getEnemySprite()->getContentSize().width * enemy->getEnemySprite()->getScaleX() * 0.5f;

		//Disable enemy if it goes below the screen
		if (enemy->getEnemySprite()->getPositionY() + spriteGameWidth < 0)
		{
			enemy->setIsActive(false);
			enemy->getEnemySprite()->pause();
			enemy->getEnemySprite()->setVisible(false);
		}
		//Player Collision

		if (mainChar.getLifeCount() > 0)
		{
			if ((enemy->getEnemySprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
			{
				//Lose a life here
				if (!mainChar.getShieldActive() && !mainChar.getInvulActive())
				{
					mainChar.setLifeCount(mainChar.getLifeCount() - 1);
					mainChar.setInvulActive(true);
				}
			}
		}
	}
}

TrapObject* HelloWorld::FetchTrapObject(const TrapObject::TRAP_TYPE trapType)
{
	//Find an inactive trap, return it if one is found

	for (auto trapObj : trapObjectList)
	{
		if (!trapObj->getIsActive())
		{
			trapObj->setTrapType(trapType);
			return trapObj;
		}
	}

	//If no available trap is found, push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		TrapObject *newTrapObj = new TrapObject(trapType);

		newTrapObj->setIsActive(false);
		trapObjectList.push_back(newTrapObj);
	}

	return FetchTrapObject(trapType);
}

ItemObject * HelloWorld::FetchItemObject(ItemObject::ITEM_TYPE itemType)
{
	//Find an inactive item, return it if one is found
	for (auto itemObj : itemObjectList)
	{
		if (!itemObj->getIsActive())
		{
			itemObj->setItemType(itemType);
			return itemObj;
		}
	}

	//If no available item is found, push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		ItemObject *newItemObj = new ItemObject(itemType);
		newItemObj->setIsActive(false);
		itemObjectList.push_back(newItemObj);
	}

	return FetchItemObject(itemType);
}

Enemy * HelloWorld::FetchEnemyObject(Enemy::ENEMY_TYPE _enemyType)
{
	// Find an inactive enemy, return it if one is found
	for (auto enemy : enemyObjectList)
	{
		if (!enemy->getIsActive())
		{
			enemy->setEnemyType(_enemyType);
			enemy->getEnemySprite()->resume();
			enemy->getEnemySprite()->setVisible(true);

			return enemy;
		}
	}

	// If no available enemy is found, create and push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		Enemy *newEnemy = new Enemy(_enemyType);

		enemyObjects->addChild(newEnemy->getEnemySprite());
		newEnemy->getEnemySprite()->pause();
		newEnemy->getEnemySprite()->setVisible(false);

		enemyObjectList.push_back(newEnemy);
	}

	return FetchEnemyObject(_enemyType);
}

void HelloWorld::GameOverUI()
{
	gameoverbackground = Sprite::create("gameoverbg.jpg");
	gameoverbackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gameoverbackground->setScaleX(2);
	gameoverbackground->setScaleY(2);
	gameoverbackground->setScaleZ(2);
	gameoverbackground->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
	gameoverbackground->setVisible(false);
	this->addChild(gameoverbackground);

	// DEAD - Game Over
	deadLabel = Label::createWithTTF("DEAD", "fonts/Marker Felt.ttf", 100, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	deadLabel->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
	this->addChild(deadLabel, 1);
	deadLabel->setVisible(false);

	instructiongameover = Label::createWithTTF("press Enter to go back to main menu", "fonts/Marker Felt.ttf", 30, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	instructiongameover->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.3f));
	this->addChild(instructiongameover, 1);
	instructiongameover->setVisible(false);

	GoldEarnedLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	GoldEarnedLabel->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.35f));
	this->addChild(GoldEarnedLabel, 1);
	GoldEarnedLabel->setVisible(false);

	gameoverrestartbutton = ui::Button::create("retry.png");
	gameoverrestartbutton->setTitleText("Button Text");
	gameoverrestartbutton->setScaleX(0.5f);
	gameoverrestartbutton->setScaleY(0.5f);
	gameoverrestartbutton->setScaleZ(0.5f);
	gameoverrestartbutton->setPosition(Vec2(playingSize.width * 0.8f, 0 + distanceLabel->getContentSize().height * 27));
	gameoverrestartbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:

		const std::string Score = std::to_string((int)mainChar.getScore());
		int GoldEarned;

		//Less than 2 digits
		if (Score.size() < 2)
		{
			GoldEarned = 0;
		}
		else
		{
			GoldEarned = std::stoi(Score.substr(0, Score.size() - 2));
		}

		SaveData::GetInstance().SetGold(SaveData::GetInstance().GetGold() + GoldEarned);

		paused = false;
		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });

	gameoverrestartbutton->setVisible(false);
	this->addChild(gameoverrestartbutton, 1);

	gameoverbackbutton = ui::Button::create("home.png");
	gameoverbackbutton->setTitleText("Button Text");
	gameoverbackbutton->setScaleX(0.5f);
	gameoverbackbutton->setScaleY(0.5f);
	gameoverbackbutton->setScaleZ(0.5f);
	gameoverbackbutton->setPosition(Vec2(playingSize.width * 0.9f, 0 + distanceLabel->getContentSize().height * 27));
	gameoverbackbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:

		const std::string Score = std::to_string((int)mainChar.getScore());
		int GoldEarned;

		//Less than 2 digits
		if (Score.size() < 2)
		{
			GoldEarned = 0;
		}
		else
		{
			GoldEarned = std::stoi(Score.substr(0, Score.size() - 2));
		}

		SaveData::GetInstance().SetGold(SaveData::GetInstance().GetGold() + GoldEarned);

		paused = false;
		auto scene = MainMenu::createScene();

		HelloWorld* HWScene = dynamic_cast<HelloWorld*>(scene);


		if (std::find(SaveData::GetInstance().BoughtNEquippedPowerUps.begin(),
			SaveData::GetInstance().BoughtNEquippedPowerUps.end(),
			"RestartWithK") != SaveData::GetInstance().BoughtNEquippedPowerUps.end())
		{
			// Found Restart with thousandth Score Power Up in Bought vector
			HWScene->getChar()->setScore(std::stof(Score.substr(0, Score.size() - 3)));

			// Remove the Power Up from the vector
			SaveData::GetInstance().BoughtNEquippedPowerUps.end() = std::remove(SaveData::GetInstance().BoughtNEquippedPowerUps.begin(),
				SaveData::GetInstance().BoughtNEquippedPowerUps.end(),
				"RestartWithK");
			SaveData::GetInstance().BoughtNEquippedPowerUps.pop_back();
		}

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });

	gameoverbackbutton->setVisible(false);
	this->addChild(gameoverbackbutton, 1);


	/*scoreLabel = Label::createWithTTF("You Scored: " + std::to_string((int)mainChar.getScore()), "fonts/Marker Felt.ttf", 24);
	scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, scoreLabel->getContentSize().height * 8));
	this->addChild(scoreLabel, 1);
	*/
}

void HelloWorld::PauseUI()
{
	pausebackground = Sprite::create("ps.png");
	pausebackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pausebackground->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
	pausebackground->setVisible(false);
	this->addChild(pausebackground, 0);

	pausebutton = ui::Button::create("pause.png");
	pausebutton->setTitleText("Button Text");
	pausebutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height * 35));
	pausebutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		if (mainChar.getAliveorNot() == false)
			paused = true;
		else
			paused = false;
		break;
		/*default:
		break; */
	} });
	pausebutton->setVisible(true);
	this->addChild(pausebutton);

	resumebutton = ui::Button::create("play.png");
	resumebutton->setTitleText("Button Text");
	resumebutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height * 20));
	resumebutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		paused = false;
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });
	resumebutton->setVisible(false);
	this->addChild(resumebutton);

	retrybutton = ui::Button::create("retry.png");
	retrybutton->setTitleText("Button Text");
	retrybutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height * 15));
	retrybutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		paused = false;
		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });

	retrybutton->setVisible(false);
	this->addChild(retrybutton);

	mainmenubutton = ui::Button::create("home.png");
	mainmenubutton->setTitleText("Button Text");
	mainmenubutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height * 5));
	mainmenubutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		paused = false;
		auto scene = MainMenu::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });

	mainmenubutton->setVisible(false);
	this->addChild(mainmenubutton);



	tutorialbutton = ui::Button::create("buttontutor.png");
	tutorialbutton->setTitleText("Button Text");
	tutorialbutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height * 10));
	tutorialbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		paused = false;
		Director::sharedDirector()->resume();
		tutormode = true;
		break;
		/*default:
		break; */
	} });

	tutorialbutton->setVisible(false);
	this->addChild(tutorialbutton);

	/*scoreLabel2 = Label::createWithTTF("Score: " + std::to_string((int)mainChar.getScore()), "fonts/Marker Felt.ttf", 24);
	scoreLabel2->setPosition(Vec2(playingSize.width * 0.5f, scoreLabel->getContentSize().height * 20));
	this->addChild(scoreLabel2, 1);
	*/
	//Resume = Label::createWithTTF("RESUME", "fonts/Marker Felt.ttf", 24);
	//Resume->setPosition(Vec2(playingSize.width * 0.5f, 0 + Resume->getContentSize().height * 15));
	//Resume->setVisible(false);
	//this->addChild(Resume, 1);

	//Restart = Label::createWithTTF("RESTART", "fonts/Marker Felt.ttf", 24);
	//Restart->setPosition(Vec2(playingSize.width * 0.5f, 0 + Restart->getContentSize().height * 13));
	//Restart->setVisible(false);
	//this->addChild(Restart, 1);

	//Tutorial = Label::createWithTTF("TUTORIAL", "fonts/Marker Felt.ttf", 24);
	//Tutorial->setPosition(Vec2(playingSize.width * 0.5f, 0 + Tutorial->getContentSize().height * 11));
	//Tutorial->setVisible(false);
	//this->addChild(Tutorial, 1);

	//MainMenu = Label::createWithTTF("MAINMENU", "fonts/Marker Felt.ttf", 24);
	//MainMenu->setPosition(Vec2(playingSize.width * 0.5f, 0 + MainMenu->getContentSize().height * 9));
	//MainMenu->setVisible(false);
	//this->addChild(MainMenu, 1);


}

void HelloWorld::Exit()
{
	for (auto go : trapObjectList)
	{
		delete go;
		go = nullptr;
	}

	for (auto go : itemObjectList)
	{
		delete go;
		go = nullptr;
	}

	for (auto go : enemyObjectList)
	{
		delete go;
		go = nullptr;
	}

	trapObjectList.clear();
	itemObjectList.clear();
	enemyObjectList.clear();

	if (LastSpawnedEnemy)
	{
		LastSpawnedEnemy = nullptr;
	}
	if (LastSpawnedItem)
	{
		LastSpawnedItem = nullptr;
	}
	if (LastSpawnedTrap)
	{
		LastSpawnedTrap = nullptr;
	}
}