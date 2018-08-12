#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "TrapObjects.h"
#include "ItemObjects.h"
#include "Enemy.h"
#include "ui\CocosGUI.h"
#include "Enemy2.h"

using namespace cocos2d;
enum Selection2
{
	RESUME = 0,
	RESTART,
	TUTORIAL2,
	MAINMENU,
};

class HelloWorld : public cocos2d::Scene
{
public:
	// For cleaning auto-spawner function
	enum ESpawner {
		eNone,
		eSamuraiEnemy,
		eCoin,
		eMagnet,
		eShield,
		eSpike,
		eShuriken,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// Controls functions
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onMouseUp(Event* event);
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual bool onTouchBegan2(Touch * touch, Event * event);
	// Update funciton that cals every frame
	virtual void update(float delta);

	//Exit function that release all memory
	void Exit();

	//Float range min mx function
	float RandomFloatRange(const float &a, const float &b);

	// Spawning Functions
	Enemy* SpawnSamuraiEnemy();
	ItemObject* SpawnCoin();
	ItemObject* SpawnMagnet();
	ItemObject* SpawnShield();
	TrapObject* SpawnSpike();
	TrapObject* SpawnShuriken();

	// Code 'Cleaning' Functions
	void GameObjectsInit();
	void LabelInit();
	void LabelUpdate();
	void AutoSpawner(float delta, float &timer, float timing, ESpawner _eSpawn);
	void WallUpdate(float delta);
	void TrapUpdate(float delta);
	void ItemUpdate(float delta);
	void EnemyUpdate(float delta);
	void PauseUI();
	void GameOverUI();
	
	//Fetchers
	TrapObject* FetchTrapObject(TrapObject::TRAP_TYPE trapType = TrapObject::TRAP_RANDOM);
	ItemObject* FetchItemObject(ItemObject::ITEM_TYPE itemType = ItemObject::ITEM_RANDOM);
	Enemy* FetchEnemyObject(Enemy::ENEMY_TYPE enemyType = Enemy::ENEMY_RANDOM);

	ItemObject* LastSpawnedItem;
	TrapObject* LastSpawnedTrap;
	Enemy* LastSpawnedEnemy;

	// Get Char
	GameChar* getChar(void) { return &mainChar; }
	GameEnemy* getEnemyChar(void) { return &enemyChar; }
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
private:
	GameChar mainChar;
	GameEnemy enemyChar;
	Node *BackgroundNode;
	Node *wallObjects;
	Node *trapObjects;
	Node *itemObjects;
	Node *enemyObjects;

	Label* scoreLabel;
	Label* distanceLabel;
	Label* magnetLabel;
	Label* lifeLabel;
	Label* shieldLabel;
	Label* invulLabel;
	Label* deadLabel;
	Label* MainMenu;
	Label* Restart;
	Label* Tutorial;
	Label* Resume;
	Label* scoreLabel2; //for pause
	Label* scoreLabel3; // for gameover
	Label* instructiongameover;
	Label *GoldEarnedLabel;

	float samuraiSpawnTimer;
	float coinSpawnTimer;
	float magnetSpawnTimer;
	float shieldSpawnTimer;
	float spikeSpawnTimer;
	float shurikenSpawnTimer;
	float enemyspawntimer;
	float characterSpriteWidth;
	float enemyspritewidth;
	float movespeed;

	Sprite* arrow2;
	Sprite* pausebackground;
	Sprite* gameoverbackground;
	Sprite* tutorialbackground;
	Selection2 selection2;
	bool paused;
	cocos2d::ui::Button* resumebutton;
	cocos2d::ui::Button* retrybutton;
	cocos2d::ui::Button* mainmenubutton;
	cocos2d::ui::Button* tutorialbutton;
	cocos2d::ui::Button* backbutton;
	cocos2d::ui::Button* pausebutton;
	cocos2d::ui::Button* gameoverbackbutton;
	cocos2d::ui::Button* gameoverrestartbutton;
	bool tutormode;

	Size playingSize;
	std::vector<TrapObject*> trapObjectList;
	std::vector<ItemObject*> itemObjectList;
	std::vector<Enemy*> enemyObjectList;
};

#endif // __HELLOWORLD_SCENE_H_