#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
USING_NS_CC;

#define WALL_MOVESPEED 250.0f
#define WALL_CONTENTSIZE_X 50.0f

#define SAMURAI_SPAWN_TIMING 3.0f
#define COIN_SPAWN_TIMING 5.0f
#define MAGNET_SPAWN_TIMING 7.0f
#define SHIELD_SPAWN_TIMING 7.0f

#define COIN_SCORE 100.0f
#define COIN_SPEED 200.0f
#define MAGNET_STRENGTH 1050.0f

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	playingSize = Size(visibleSize.width, visibleSize.height);

	// Init all game objects(walls,traps,items,enemies,player)
	GameObjectsInit();

	// Init all labels(text on screen)
	LabelInit();

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
			float LTarget = WALL_CONTENTSIZE_X * 0.5f;
			float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 0.5f);

			mainChar.Jump(LTarget, RTarget, playingSize.height / 2);
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

void HelloWorld::update(float delta)
{
	LabelUpdate();
	AutoSpawner(delta);

	// DEAD - GAME OVER
	if (mainChar.getLifeCount() <= 0)
	{
		deadLabel->setVisible(true);
		mainChar.getSprite()->pause();
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
}

void HelloWorld::SpawnSamuraiEnemy()
{
	Enemy* Enemy = FetchEnemyObject(Enemy::ENEMY_SAMURAI);
	Enemy->setIsActive(true);
	Enemy->getEnemySprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	// Random choose to spawn at left side or right side
	int random_dir = RandomHelper::random_int(0, 9);
	if(random_dir >= 5)
		Enemy->setEnemyDirection(Enemy::ENEMY_RIGHT);
	else
		Enemy->setEnemyDirection(Enemy::ENEMY_LEFT);

	// Set position according to Enemy direction
	switch (Enemy->getEnemyDirection())
	{
	case Enemy::ENEMY_RIGHT:
		Enemy->getEnemySprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height * 2));
		break;
	case Enemy::ENEMY_LEFT:
		Enemy->getEnemySprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height * 2));
		break;
	}

	// Call Run animating Function
	Enemy->Run();
}

void HelloWorld::SpawnCoin()
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
		Coin->setIsActive(true);
		Coin->getItemSprite()->resume();
		if(!Coin->getItemSprite()->isVisible())
			Coin->getItemSprite()->setVisible(true);
		Coin->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		Coin->setCoinScore(COIN_SCORE);

		if (random_dir >= 5)
			Coin->setItemDirection(ItemObject::ITEM_RIGHT);
		else
			Coin->setItemDirection(ItemObject::ITEM_LEFT);

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
	}
}

void HelloWorld::SpawnMagnet()
{
	ItemObject* Magnet = FetchItemObject(ItemObject::ITEM_MAGNET);
	Magnet->setItemSprite(Sprite::create("magnet.png"));
	Magnet->setIsActive(true);
	Magnet->getItemSprite()->resume();
	if (!Magnet->getItemSprite()->isVisible())
		Magnet->getItemSprite()->setVisible(true);
	Magnet->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	// Random choose to spawn at left side or right side
	int random_dir = RandomHelper::random_int(0, 9);
	if (random_dir >= 5)
		Magnet->setItemDirection(ItemObject::ITEM_RIGHT);
	else
		Magnet->setItemDirection(ItemObject::ITEM_LEFT);

	// Set position according to Item direction
	switch (Magnet->getItemDirection())
	{
	case ItemObject::ITEM_RIGHT:
		Magnet->getItemSprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height));
		Magnet->getItemSprite()->setRotation(-90);
		break;
	case ItemObject::ITEM_LEFT:
		Magnet->getItemSprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height));
		Magnet->getItemSprite()->setRotation(90);
		break;
	}
	itemObjects->addChild(Magnet->getItemSprite());
}

void HelloWorld::SpawnShield()
{
	ItemObject* Shield = FetchItemObject(ItemObject::ITEM_SHIELD);
	Shield->setItemSprite(Sprite::create("shield.png"));
	Shield->setIsActive(true);
	Shield->getItemSprite()->resume();
	if (!Shield->getItemSprite()->isVisible())
		Shield->getItemSprite()->setVisible(true);
	Shield->getItemSprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	// Random choose to spawn at left side or right side
	int random_dir = RandomHelper::random_int(0, 9);
	if (random_dir >= 5)
		Shield->setItemDirection(ItemObject::ITEM_RIGHT);
	else
		Shield->setItemDirection(ItemObject::ITEM_LEFT);

	// Set position according to Item direction
	switch (Shield->getItemDirection())
	{
	case ItemObject::ITEM_RIGHT:
		Shield->getItemSprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height));
		Shield->getItemSprite()->setRotation(-90);
		break;
	case ItemObject::ITEM_LEFT:
		Shield->getItemSprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height));
		Shield->getItemSprite()->setRotation(90);
		break;
	}
	itemObjects->addChild(Shield->getItemSprite());
}

void HelloWorld::GameObjectsInit()
{
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

	//TrapObject* test = FetchTrapObject(TrapObject::TRAP_SPIKES);
	//test->isActive = true;

	//Set the position of reap
	//test->trapSprite->setPosition(Vec2(50, playingSize.height * 2));
	// ------------------ Init Traps ----------------------

	// ------------------ Init Items ----------------------
	itemObjects = Node::create();
	itemObjects->setName("ItemObjects");
	this->addChild(itemObjects);
	coinSpawnTimer = 0.0f;
	magnetSpawnTimer = 0.0f;
	shieldSpawnTimer = 0.0f;
	// ------------------ Init Items ----------------------

	// ------------------ Init Enemy ----------------------
	enemyObjects = Node::create();
	enemyObjects->setName("EnemyObjects");
	this->addChild(enemyObjects);
	samuraiSpawnTimer = 0.0f;
	// ------------------ Init Enemy ----------------------

	// ------------------ Init Player ----------------------
	auto playerObject = Node::create();
	playerObject->setName("PlayerObject");
	auto playerSprite = Sprite::create("run_right_01.png");
	mainChar.init("run_right_01.png", Vec2::ANCHOR_MIDDLE_BOTTOM, (playingSize.width - (WALL_CONTENTSIZE_X * 0.5f)), (playerSprite->getContentSize().width * 2), "Player");
	playerObject->addChild(mainChar.getSprite(), 1);
	this->addChild(playerObject, 1);
	characterSpriteWidth = mainChar.getSprite()->getContentSize().width * mainChar.getSprite()->getScaleX();

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

	// DEAD - Game Over
	deadLabel = Label::createWithTTF("DEAD", "fonts/Marker Felt.ttf", 100, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	deadLabel->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.5f));
	this->addChild(deadLabel, 1);
	deadLabel->setVisible(false);
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
	{
		shieldLabel->setString("Shield Left: " + std::to_string((int)(mainChar.getShieldDuration() - mainChar.getShieldTimer())));
		mainChar.getShieldSprite()->setPosition(mainChar.getSprite()->getPosition());
	}
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

void HelloWorld::AutoSpawner(float delta)
{
	// Spawn Samurai Enemy
	samuraiSpawnTimer += 1 * delta;
	if (samuraiSpawnTimer >= SAMURAI_SPAWN_TIMING)
	{
		SpawnSamuraiEnemy();
		samuraiSpawnTimer = 0.0f;
	}

	// Spawn Coin
	coinSpawnTimer += 1 * delta;
	if (coinSpawnTimer >= COIN_SPAWN_TIMING)
	{
		SpawnCoin();
		coinSpawnTimer = 0.0f;
	}

	// Spawn Magnet
	magnetSpawnTimer += 1 * delta;
	if (magnetSpawnTimer >= MAGNET_SPAWN_TIMING)
	{
		SpawnMagnet();
		magnetSpawnTimer = 0.0f;
	}

	// Spawn Shield
	shieldSpawnTimer += 1 * delta;
	if (shieldSpawnTimer >= SHIELD_SPAWN_TIMING)
	{
		SpawnShield();
		shieldSpawnTimer = 0.0f;
	}
}

void HelloWorld::WallUpdate(float delta)
{
	//Get the array of walls from wallObjects node
	static auto wallObjectsArray = wallObjects->getChildren();

	//Move each wall sprite downwards
	for (auto wallSprite : wallObjectsArray)
	{
		if (wallSprite->getPositionY() < -playingSize.height * 0.5f)
		{
			wallSprite->setPositionY((playingSize.height * 0.5f) + playingSize.height);
		}
		else
		{
			wallSprite->setPositionY(wallSprite->getPositionY() - WALL_MOVESPEED * delta);
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

		//Update trap
		trapObj->TrapUpdate(delta);

		const float spriteGameWidth = (trapObj->gettrapSprite()->getContentSize().width) * (trapObj->gettrapSprite()->getScaleX());

		//Disable trap if it goes below the screen
		if (trapObj->gettrapSprite()->getPositionY() + spriteGameWidth < 0)
		{
			trapObj->setIsActive(false);
			trapObj->gettrapSprite()->pause();
			trapObj->gettrapSprite()->setVisible(false);
		}
		//Collision
		if ((trapObj->gettrapSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
		{
			//Lose a life here
			if (!mainChar.getShieldActive() && !mainChar.getInvulActive())
			{
				if (mainChar.getLifeCount() > 0)
				{
					mainChar.setLifeCount(mainChar.getLifeCount() - 1);
					mainChar.setInvulActive(true);
				}
			}
		}
	}
}

void HelloWorld::ItemUpdate(float delta)
{
	//Update each item in item list & collision check
	for (auto itemObj : itemObjectList)
	{
		if (!itemObj->getIsActive())
		{
			continue;
		}

		//Update item
		itemObj->ItemUpdate(delta);

		// Update Coin
		if (itemObj->getItemType() == ItemObject::ITEM_COIN)
		{
			if (mainChar.getMagnetActive())
			{
				// Magnet sucks coins to character
				if (itemObj->getItemSprite()->isVisible() &&
					(itemObj->getItemSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= MAGNET_STRENGTH)
				{
					Vec2 dir = (mainChar.getSprite()->getPosition() - itemObj->getItemSprite()->getPosition()).getNormalized();
					dir *= MAGNET_STRENGTH * delta;

					itemObj->getItemSprite()->setPosition(itemObj->getItemSprite()->getPosition() + dir);
				}
				else
					itemObj->getItemSprite()->setPositionY(itemObj->getItemSprite()->getPositionY() - COIN_SPEED * delta);
			}
			else
			{
				itemObj->getItemSprite()->setPositionY(itemObj->getItemSprite()->getPositionY() - COIN_SPEED * delta);
			}
		}
		const float spriteGameWidth = itemObj->getItemSprite()->getContentSize().width * itemObj->getItemSprite()->getScaleX();

		//Disable item if it goes below the screen
		if (itemObj->getItemSprite()->getPositionY() + spriteGameWidth < 0)
		{
			itemObj->setIsActive(false);
			itemObj->getItemSprite()->pause();
			itemObj->getItemSprite()->setVisible(false);
		}
		//Player Collision
		else if ((itemObj->getItemSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
		{
			switch (itemObj->getItemType())
			{
			case ItemObject::ITEM_COIN:
			{
				mainChar.setScore(mainChar.getScore() + itemObj->getCoinScore());
				break;
			}
			case ItemObject::ITEM_SHIELD:
			{
				if (!mainChar.getShieldActive())
				{
					mainChar.setShieldActive(true);
					mainChar.getShieldSprite()->setVisible(true);
				}
				break;
			}
			case ItemObject::ITEM_MAGNET:
			{
				if (!mainChar.getMagnetActive())
					mainChar.setMagnetActive(true);
				break;
			}
			}

			itemObj->setIsActive(false);
			itemObj->getItemSprite()->pause();
			itemObj->getItemSprite()->setVisible(false);
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
		enemy->EnemyUpdate(delta);

		const float spriteGameWidth = enemy->getEnemySprite()->getContentSize().width * enemy->getEnemySprite()->getScaleX();

		//Disable trap if it goes below the screen
		if (enemy->getEnemySprite()->getPositionY() + spriteGameWidth < 0)
		{
			enemy->setIsActive(false);
			enemy->getEnemySprite()->pause();
			enemy->getEnemySprite()->setVisible(false);
		}
		//Player Collision
		else if ((enemy->getEnemySprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
		{
			//Lose a life here
			if (!mainChar.getShieldActive() && !mainChar.getInvulActive())
			{
				if (mainChar.getLifeCount() > 0)
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
			trapObj->settrapType(trapType);
			trapObj->gettrapSprite()->resume();
			trapObj->gettrapSprite()->setVisible(true);
			
			return trapObj;
		}
	}

	//If no available trap is found, push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		TrapObject *newTrapObj = new TrapObject(trapType);
		
		newTrapObj->setIsActive(false);
		trapObjects->addChild(newTrapObj->gettrapSprite());
		newTrapObj->gettrapSprite()->pause();
		newTrapObj->gettrapSprite()->setVisible(false);

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

	//If no available trap is found, push 10 new ones into list
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
