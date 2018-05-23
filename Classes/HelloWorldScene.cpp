#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
USING_NS_CC;

#define WALL_MOVESPEED 250
#define WALL_CONTENTSIZE_X 50
#define SAMURAI_SPAWN_TIMING 3

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

	//Create the walls that player runs on
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

	//Create Traps
	trapObjects = Node::create();
	trapObjects->setName("TrapObjects");
	this->addChild(trapObjects);

	TrapObject* test = FetchTrapObject(TrapObject::TRAP_SPIKES);
	test->isActive = true;

	//Set the position of reap
	//test->trapSprite->setPosition(Vec2(50, playingSize.height * 2));

	//Create items
	itemObjects = Node::create();
	itemObjects->setName("ItemObjects");
	this->addChild(itemObjects);

	// Init Enemy
	enemyObjects = Node::create();
	enemyObjects->setName("EnemyObjects");
	this->addChild(enemyObjects);
	samuraiSpawnTimer = 0.0f;

	/*
	***********************
	***** Player Init *****
	***********************
	*/
	auto playerObject = Node::create();
	playerObject->setName("PlayerObject");

	auto playerSprite = Sprite::create("run_right_01.png");

	mainChar.init("run_right_01.png", Vec2::ANCHOR_MIDDLE_BOTTOM, (playingSize.width - (WALL_CONTENTSIZE_X * 0.5f)), (playerSprite->getContentSize().width * 2), "Player");
	playerObject->addChild(mainChar.getSprite(), 1);
	this->addChild(playerObject, 1);

	distanceLabel = Label::createWithTTF("Distance Travelled: " + std::to_string(mainChar.getDistanceTravelled()), "fonts/Marker Felt.ttf", 24);
	distanceLabel->setPosition(Vec2(playingSize.width * 0.5f, 0 + distanceLabel->getContentSize().height));
	this->addChild(distanceLabel, 1);

	scoreLabel = Label::createWithTTF("Score: " + std::to_string(mainChar.getScore()), "fonts/Marker Felt.ttf", 24);
	scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, 0 + scoreLabel->getContentSize().height + distanceLabel->getContentSize().height));
	this->addChild(scoreLabel, 1);

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
	//mainChar.MoveChar(eStop);

	// Debug Code - Exit application
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}

	// Jump to the opposite side
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		// Only allow Jumping when Status is eRun
		if (mainChar.getStatus() == eRun)
		{
			mainChar.setStatus(eJump);
			float LTarget = WALL_CONTENTSIZE_X * 0.5f;
			float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 0.5f);

			mainChar.Jump(LTarget, RTarget, playingSize.height / 2);
		}
	}

	// Spawn Samurai enemy
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		SpawnSamuraiEnemy();
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
	// Update Character
	scoreLabel->setString("Score: " + std::to_string(mainChar.getScore()));
	distanceLabel->setString("Distance Travelled: " + std::to_string(mainChar.getDistanceTravelled()));
	mainChar.Update(delta);

	// Update Wall
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

	// Spawn Samurai Enemy
	samuraiSpawnTimer += 1 * delta;
	if (samuraiSpawnTimer >= SAMURAI_SPAWN_TIMING)
	{
		SpawnSamuraiEnemy();
		samuraiSpawnTimer = 0.0f;
	}

	static const float characterSpriteWidth = mainChar.getSprite()->getContentSize().width * mainChar.getSprite()->getScaleX();

	//Update each trap in trap list & collision check
	for (auto trapObj : trapObjectList)
	{
		if (!trapObj->isActive)
		{
			continue;
		}

		//Update trap
		trapObj->TrapUpdate(delta);
		
		const float spriteGameWidth = trapObj->trapSprite->getContentSize().width * trapObj->trapSprite->getScaleX();

		//Disable trap if it goes below the screen
		if (trapObj->trapSprite->getPositionY() + spriteGameWidth < 0)
		{
			trapObj->isActive = false;
			trapObj->trapSprite->pause();
			trapObj->trapSprite->setVisible(false);
		}
		//Collision
		if ((trapObj->trapSprite->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
		{
			//Lose a life here
		}
	}

	//Update each item in item list & collision check
	for (auto itemObj : itemObjectList)
	{
		if (!itemObj->isActive)
		{
			continue;
		}

		//Update item
		itemObj->ItemUpdate(delta);

		const float spriteGameWidth = itemObj->itemSprite->getContentSize().width * itemObj->itemSprite->getScaleX();

		//Disable trap if it goes below the screen
		if (itemObj->itemSprite->getPositionY() + spriteGameWidth < 0)
		{
			itemObj->isActive = false;
			itemObj->itemSprite->pause();
			itemObj->itemSprite->setVisible(false);
		}
		//Player Collision
		else if ((itemObj->itemSprite->getPosition() - mainChar.getSprite()->getPosition()).length() <= spriteGameWidth + characterSpriteWidth)
		{
			switch (itemObj->itemType)
			{
			case ItemObject::ITEM_COIN:
			{
				
				break;
			}
			case ItemObject::ITEM_SHIELD:
			{
				break;
			}
			case ItemObject::ITEM_MAGNET:
			{
				break;
			}
			default:
				break;
			}

			itemObj->isActive = false;
			itemObj->itemSprite = nullptr;
		}
	}

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
		}
	}
}

void HelloWorld::SpawnSamuraiEnemy()
{
	Enemy* testEnemy = FetchEnemyObject(Enemy::ENEMY_SAMURAI);
	testEnemy->setIsActive(true);
	testEnemy->getEnemySprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	// Random choose to spawn at left side or right side
	int random_dir = RandomHelper::random_int(0, 9);
	if(random_dir >= 5)
		testEnemy->setEnemyDirection(Enemy::ENEMY_RIGHT);
	else
		testEnemy->setEnemyDirection(Enemy::ENEMY_LEFT);

	// Set position according to Enemy direction
	switch (testEnemy->getEnemyDirection())
	{
	case Enemy::ENEMY_RIGHT:
		testEnemy->getEnemySprite()->setPosition(Vec2((playingSize.width - (WALL_CONTENTSIZE_X * 0.4f)), playingSize.height * 2));
		break;
	case Enemy::ENEMY_LEFT:
		testEnemy->getEnemySprite()->setPosition(Vec2(WALL_CONTENTSIZE_X * 0.4f, playingSize.height * 2));
		break;
	}

	// Call Run animating Function
	testEnemy->Run();
}

TrapObject* HelloWorld::FetchTrapObject(const TrapObject::TRAP_TYPE trapType)
{
	//Find an inactive trap, return it if one is found
	
	for (auto trapObj : trapObjectList)
	{
		if (!trapObj->isActive)
		{
			trapObj->trapType = trapType;
			trapObj->trapSprite->resume();
			trapObj->trapSprite->setVisible(true);
			
			return trapObj;
		}
	}

	//If no available trap is found, push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		TrapObject *newTrapObj = new TrapObject(trapType);
		
		newTrapObj->isActive = false;
		trapObjects->addChild(newTrapObj->trapSprite);
		newTrapObj->trapSprite->pause();
		newTrapObj->trapSprite->setVisible(false);

		trapObjectList.push_back(newTrapObj);
	}

	return FetchTrapObject(trapType);
}

ItemObject * HelloWorld::FetchItemObject(ItemObject::ITEM_TYPE itemType)
{
	//Find an inactive item, return it if one is found
	for (auto itemObj : itemObjectList)
	{
		if (!itemObj->isActive)
		{
			itemObj->itemType = itemType;
			itemObj->itemSprite->resume();
			itemObj->itemSprite->setVisible(true);
			
			return itemObj;
		}
	}

	//If no available trap is found, push 10 new ones into list
	for (size_t i = 0; i < 10; ++i)
	{
		ItemObject *newItemObj = new ItemObject(itemType);
		
		newItemObj->isActive = false;
		itemObjects->addChild(newItemObj->itemSprite);
		newItemObj->itemSprite->pause();
		newItemObj->itemSprite->setVisible(false);

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

GameChar* HelloWorld::getChar()
{
	return &mainChar;
}
