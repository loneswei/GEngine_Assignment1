#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
#include "ui\CocosGUI.h"
#include <string>
USING_NS_CC;
using std::string;

#define WALL_MOVESPEED 250.0f
#define WALL_CONTENTSIZE_X 50.0f

#define SAMURAI_SPAWN_TIMING 3.0f
#define COIN_SPAWN_TIMING 5.0f
#define MAGNET_SPAWN_TIMING 7.0f
#define SHIELD_SPAWN_TIMING 7.0f

#define COIN_SCORE 100.0f
#define COIN_SPEED 200.0f
#define MAGNET_STRENGTH 1050.0f

Scene* LoadingScene::createScene()
{
	// return scene
	return LoadingScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto label = Label::createWithTTF("Loading Next Scene", "fonts/Marker Felt.ttf", 24);

	if (label != nullptr)
	{
		//position the label on the center of the screen
		label->setPosition(Vec2((int)visibleSize.width >> 1, (int)visibleSize.height >> 1));

		//add the label as a child to this layer

		this->addChild(label, 1);
	}

	//Preload Audio Files
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	//BGM
	audio->preloadBackgroundMusic("Audio/Bgm/Menu.mp3");
	audio->preloadBackgroundMusic("Audio/Bgm/GameOver.mp3");
	audio->preloadBackgroundMusic("Audio/Bgm/GamePlay.mp3");
	audio->preloadBackgroundMusic("Audio/Bgm/SpeedUp.mp3");
	//Sound Effects
	audio->preloadEffect("Audio/SoundEffect/HeroDeath.mp3");
	audio->preloadEffect("Audio/SoundEffect/coin_pickup.wav");
	audio->preloadEffect("Audio/SoundEffect/shield_pickup.mp3");
	audio->preloadEffect("Audio/SoundEffect/magnet_pickup.mp3");

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Resources[0] = "jump_left_01.png";
	Resources[1] = "jump_left_02.png";
	Resources[2] = "jump_left_03.png";
	Resources[3] = "jump_left_04.png";
	Resources[4] = "jump_left_05.png";

	Resources[5] = "jump_right_01.png";
	Resources[6] = "jump_right_02.png";
	Resources[7] = "jump_right_03.png";
	Resources[8] = "jump_right_04.png";
	Resources[9] = "jump_right_05.png";

	Resources[10] = "GameBackground.jpg";
	Resources[11] = "gameoverbg.jpg";
	Resources[12] = "tutorial.png";
	Resources[13] = "woodwall.png";

	Resources[14] = "spiketrap.png";
	Resources[15] = "shuriken.png";
	Resources[16] = "shield.png";
	Resources[17] = "ps.png";

	Resources[17] = "mainmenubackground.jpg";
	Resources[18] = "magnet.png";
	Resources[19] = "load.png";

	Resources[20] = "run_left_01.png";
	Resources[21] = "run_left_02.png";
	Resources[22] = "run_left_03.png";
	Resources[23] = "run_left_04.png";
	Resources[24] = "run_left_05.png";
	Resources[25] = "run_left_06.png";
	Resources[26] = "run_left_07.png";

	Resources[27] = "run_right_01.png";
	Resources[28] = "run_right_02.png";
	Resources[29] = "run_right_03.png";
	Resources[30] = "run_right_04.png";
	Resources[31] = "run_right_05.png";
	Resources[32] = "run_right_06.png";
	Resources[33] = "run_right_07.png";
	Resources[34] = "run_left_07.png";
	Resources[35] = "run_left_07.png";

	Resources[36] = "samurai_run_left_01.png";
	Resources[37] = "samurai_run_left_02.png";
	Resources[38] = "samurai_run_left_03.png";
	Resources[39] = "samurai_run_left_04.png";
	Resources[40] = "samurai_run_left_05.png";
	Resources[41] = "samurai_run_left_06.png";
	Resources[42] = "samurai_run_left_07.png";
	Resources[43] = "samurai_run_left_08.png";
	Resources[44] = "samurai_run_left_09.png";
	Resources[45] = "samurai_run_left_10.png";

	Resources[46] = "samurai_run_right_01.png";
	Resources[47] = "samurai_run_right_02.png";
	Resources[48] = "samurai_run_right_03.png";
	Resources[49] = "samurai_run_right_04.png";
	Resources[50] = "samurai_run_right_05.png";
	Resources[51] = "samurai_run_right_06.png";
	Resources[52] = "samurai_run_right_07.png";
	Resources[53] = "samurai_run_right_08.png";
	Resources[54] = "samurai_run_right_09.png";
	Resources[55] = "samurai_run_right_10.png";



	numOfResources = 56;
	curResource = 0;
	playingSize = Size(visibleSize.width, visibleSize.height);

	// Init all game objects(walls,traps,items,enemies,player)
	//GameObjectsInit();

	// Init all labels(text on screen)
	//LabelInit();

	// Key Pressed movement
	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(LoadingScene::onKeyPressed, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//// Key Released movement
	//auto listener2 = EventListenerKeyboard::create();
	//listener2->onKeyReleased = CC_CALLBACK_2(LoadingScene::onKeyReleased, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	//// Mouse button Up
	//auto listener3 = EventListenerMouse::create();
	//listener3->onMouseUp = CC_CALLBACK_1(LoadingScene::onMouseUp, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);

	textLabel = Label::createWithTTF("percent: 100", "fonts/Marker Felt.ttf", 24);
	textLabel->setPosition(Vec2(playingSize.width * 0.5f, textLabel->getContentSize().height + textLabel->getContentSize().height));
	//textLabel->retain();
	this->addChild(textLabel, 1);


	auto director = Director::getInstance();
	TextureCache* textureCache = director->getTextureCache();
	textureCache->addImageAsync(Resources[0], CC_CALLBACK_1(LoadingScene::loadingTextureFinished, this));

	// Call Update function
	//this->scheduleUpdate();

	return true;
}


void LoadingScene::menuCloseCallback(Ref* pSender)
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

void LoadingScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
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

void LoadingScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	// Debug Code - Exit application
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}

	// Debug Code - Spawn Samurai Enemy
	//if (keyCode == EventKeyboard::KeyCode::KEY_S)
	//{
	//	SpawnSamuraiEnemy();
	//}

	//// Debug Code - Spawn Coin
	//if (keyCode == EventKeyboard::KeyCode::KEY_C)
	//{
	//	SpawnCoin();
	//}

	//// Debug Code - Spawn Magnet
	//if (keyCode == EventKeyboard::KeyCode::KEY_M)
	//{
	//	SpawnMagnet();
	//}

	//// Jump to the opposite side
	//if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	//{
	//	// Only allow Jumping when Status is eRun(a.k.a when player lands on wall)
	//	if (mainChar.getStatus() == mainChar.eRun)
	//	{
	//		mainChar.setStatus(mainChar.eJump);
	//		float LTarget = WALL_CONTENTSIZE_X * 0.5f;
	//		float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 0.5f);

	//		mainChar.Jump(LTarget, RTarget, playingSize.height / 2);
	//	}
	//}
	////pause and resume here
	//if (keyCode == EventKeyboard::KeyCode::KEY_P)
	//{
	//	Director::sharedDirector()->pause();
	//	paused = true;
	//	PauseUI();
	//}
	//if (keyCode == EventKeyboard::KeyCode::KEY_N)
	//{
	//	Director::sharedDirector()->resume();
	//}



}

void LoadingScene::onMouseUp(Event * event)
{
	//EventMouse* e = (EventMouse*)event;
	//float x = e->getCursorX();
	//float y = e->getCursorY();

	//mainChar.MoveCharByCoord(x, y);
}

void LoadingScene::update(float delta)
{
	auto scene = HelloWorld::createScene();
	auto director = Director::getInstance();
	return director->replaceScene(scene);

	//LabelUpdate();
	//AutoSpawner(delta);

	// DEAD - GAME OVER
	//if (mainChar.getLifeCount() <= 0)
	//{
	//	//deadLabel->setVisible(true);
	//	mainChar.getSprite()->pause();
	//}

	// Update Character
	//mainChar.Update(delta);

	// Update Wall
	//WallUpdate(delta);

	// Update Traps
	//TrapUpdate(delta);

	// Update Items
	//ItemUpdate(delta);

	// Update Enemies
	//EnemyUpdate(delta);
}

void LoadingScene::loadingTextureFinished(Texture2D* texture)
{
	auto director = Director::getInstance();
	curResource++;

	//update the % in the loading screen

	int percentage = curResource * 100 / numOfResources;
	//textLabel->setString("hello");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	textLabel->setString(CCString::createWithFormat("%d%%", percentage)->getCString());
	auto loadingbar = ui::LoadingBar::create("Load.png");
	loadingbar->setPosition(visibleSize / 2);
	//loadingbar->setRotation(90);
	loadingbar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingbar->setPercent(0);
	this->addChild(loadingbar);
	if (curResource < numOfResources)
	{
		//load text resource

		TextureCache* textureCache = director->getTextureCache();
		textureCache->addImageAsync(Resources[curResource], CC_CALLBACK_1(LoadingScene::loadingTextureFinished, this));
		loadingbar->setPercent(percentage);
	}
	else
	{
		auto scene = MainMenu::createScene();
		return director->replaceScene(scene);
	}
}




//{
//	// Score
//	scoreLabel->setString("Score: " + std::to_string((int)mainChar.getScore()));
//
//	// Distance Travelled
//	distanceLabel->setString("Distance Travelled: " + std::to_string((int)mainChar.getDistanceTravelled()));
//
//	// Life Count
//	lifeLabel->setString("Life: " + std::to_string(mainChar.getLifeCount()));
//
//	// Shield Duration
//	if (mainChar.getShieldActive())
//	{
//		shieldLabel->setString("Shield Left: " + std::to_string((int)(mainChar.getShieldDuration() - mainChar.getShieldTimer())));
//		mainChar.getShieldSprite()->setPosition(mainChar.getSprite()->getPosition());
//	}
//	else
//		shieldLabel->setString("");
//
//	// Magnet Duration
//	if (mainChar.getMagnetActive())
//	{
//		magnetLabel->setString("Magnet Left: " + std::to_string((int)(mainChar.getMagnetDuration() - mainChar.getMagnetTimer())));
//		if (!mainChar.getShieldActive())
//			magnetLabel->setPosition(Vec2(magnetLabel->getPositionX(), magnetLabel->getPositionY() - shieldLabel->getContentSize().height));
//		else
//			magnetLabel->setPosition(Vec2(playingSize.width * 0.5f, magnetLabel->getContentSize().height + distanceLabel->getContentSize().height + scoreLabel->getContentSize().height + lifeLabel->getContentSize().height + shieldLabel->getContentSize().height));
//	}
//	else
//		magnetLabel->setString("");
//
//	// Invulnability Duration
//	if (mainChar.getInvulActive())
//		invulLabel->setString("invul Left: " + std::to_string((int)(mainChar.getInvulDuration() - mainChar.getInvulTimer())));
//	else
//		invulLabel->setString("");
//}
//
//void LoadingScene::AutoSpawner(float delta)
//{
//	// Spawn Samurai Enemy
//	samuraiSpawnTimer += 1 * delta;
//	if (samuraiSpawnTimer >= SAMURAI_SPAWN_TIMING)
//	{
//		SpawnSamuraiEnemy();
//		samuraiSpawnTimer = 0.0f;
//	}
//
//	// Spawn Coin
//	coinSpawnTimer += 1 * delta;
//	if (coinSpawnTimer >= COIN_SPAWN_TIMING)
//	{
//		SpawnCoin();
//		coinSpawnTimer = 0.0f;
//	}
//
//	// Spawn Magnet
//	magnetSpawnTimer += 1 * delta;
//	if (magnetSpawnTimer >= MAGNET_SPAWN_TIMING)
//	{
//		SpawnMagnet();
//		magnetSpawnTimer = 0.0f;
//	}
//
//	// Spawn Shield
//	shieldSpawnTimer += 1 * delta;
//	if (shieldSpawnTimer >= SHIELD_SPAWN_TIMING)
//	{
//		SpawnShield();
//		shieldSpawnTimer = 0.0f;
//	}
//}
//
//void LoadingScene::WallUpdate(float delta)
//{
//	//Get the array of walls from wallObjects node
//	static auto wallObjectsArray = wallObjects->getChildren();
//
//	//Move each wall sprite downwards
//	for (auto wallSprite : wallObjectsArray)
//	{
//		if (wallSprite->getPositionY() < -playingSize.height * 0.5f)
//		{
//			wallSprite->setPositionY((playingSize.height * 0.5f) + playingSize.height);
//		}
//		else
//		{
//			wallSprite->setPositionY(wallSprite->getPositionY() - WALL_MOVESPEED * delta);
//		}
//	}
//}
//
//void LoadingScene::TrapUpdate(float delta)
//{
//	//Update each trap in trap list & collision check
//	for (auto trapObj : trapObjectList)
//	{
//		if (!trapObj->getIsActive())
//		{
//			continue;
//		}
//
//		//Update trap
//		trapObj->TrapUpdate(delta);
//
//		const float spriteGameWidth = (trapObj->gettrapSprite()->getContentSize().width) * (trapObj->gettrapSprite()->getScaleX());
//
//		//Disable trap if it goes below the screen
//		if (trapObj->gettrapSprite()->getPositionY() + spriteGameWidth < 0)
//		{
//			trapObj->setIsActive(false);
//			trapObj->gettrapSprite()->pause();
//			trapObj->gettrapSprite()->setVisible(false);
//		}
//		//Collision
//		if ((trapObj->gettrapSprite()->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
//		{
//			//Lose a life here
//			if (!mainChar.getShieldActive() && !mainChar.getInvulActive())
//			{
//				if (mainChar.getLifeCount() > 0)
//				{
//					mainChar.setLifeCount(mainChar.getLifeCount() - 1);
//					mainChar.setInvulActive(true);
//				}
//			}
//		}
//	}
//}
//
//void LoadingScene::ItemUpdate(float delta)



