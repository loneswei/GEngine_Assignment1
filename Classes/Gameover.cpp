#include "Gameover.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
#include "ui/CocosGUI.h"
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

Scene* Gameover::createScene()
{
	// return scene
	return Gameover::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameoverScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Gameover::init()
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

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Resources[0] = "jump_left_01.png";
	Resources[1] = "jump_left_02.png";
	Resources[2] = "jump_left_03.png";
	Resources[3] = "jump_left_04.png";
	Resources[4] = "jump_left_05.png";

	numOfResources = 5;
	curResource = 0;
	playingSize = Size(visibleSize.width, visibleSize.height);

	// Init all game objects(walls,traps,items,enemies,player)
	//GameObjectsInit();

	// Init all labels(text on screen)
	//LabelInit();

	// Key Pressed movement
	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(Gameover::onKeyPressed, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//// Key Released movement
	//auto listener2 = EventListenerKeyboard::create();
	//listener2->onKeyReleased = CC_CALLBACK_2(Gameover::onKeyReleased, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	//// Mouse button Up
	//auto listener3 = EventListenerMouse::create();
	//listener3->onMouseUp = CC_CALLBACK_1(Gameover::onMouseUp, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);

	textLabel = Label::createWithTTF("percent: 100", "fonts/Marker Felt.ttf", 24);
	textLabel->setPosition(Vec2(playingSize.width * 0.5f, textLabel->getContentSize().height + textLabel->getContentSize().height));
	//textLabel->retain();
	this->addChild(textLabel, 1);


	auto director = Director::getInstance();
	TextureCache* textureCache = director->getTextureCache();
	textureCache->addImageAsync(Resources[0], CC_CALLBACK_1(Gameover::loadingTextureFinished, this));

	// Call Update function
	//this->scheduleUpdate();

	return true;
}


void Gameover::menuCloseCallback(Ref* pSender)
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

void Gameover::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
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

void Gameover::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
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

void Gameover::onMouseUp(Event * event)
{
	//EventMouse* e = (EventMouse*)event;
	//float x = e->getCursorX();
	//float y = e->getCursorY();

	//mainChar.MoveCharByCoord(x, y);
}

void Gameover::update(float delta)
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

void Gameover::loadingTextureFinished(Texture2D* texture)
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
		textureCache->addImageAsync(Resources[curResource], CC_CALLBACK_1(Gameover::loadingTextureFinished, this));
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
//void Gameover::AutoSpawner(float delta)
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
//void Gameover::WallUpdate(float delta)
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
//void Gameover::TrapUpdate(float delta)
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
//void Gameover::ItemUpdate(float delta)



