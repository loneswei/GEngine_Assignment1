#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "TrapObjects.h"
#include "ItemObjects.h"
#include "Enemy.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	// Controls functions
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onMouseUp(Event* event);

	// Update funciton that cals every frame
	virtual void update(float delta);

	// Spawning Functions
	void SpawnSamuraiEnemy();
	void SpawnCoin();
	void SpawnMagnet();
	void SpawnShield();

	// Code 'Cleaning' Functions
	void GameObjectsInit();
	void LabelInit();
	void LabelUpdate();
	void AutoSpawner(float delta);
	void WallUpdate(float delta);
	void TrapUpdate(float delta);
	void ItemUpdate(float delta);
	void EnemyUpdate(float delta);

	//Fetchers
	TrapObject* FetchTrapObject(TrapObject::TRAP_TYPE trapType = TrapObject::TRAP_RANDOM);
	ItemObject* FetchItemObject(ItemObject::ITEM_TYPE itemType = ItemObject::ITEM_RANDOM);
	Enemy* FetchEnemyObject(Enemy::ENEMY_TYPE enemyType = Enemy::ENEMY_RANDOM);

	// Get Char
	GameChar* getChar(void) { return &mainChar; }

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	GameChar mainChar;
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

	float samuraiSpawnTimer;
	float coinSpawnTimer;
	float magnetSpawnTimer;
	float shieldSpawnTimer;
	float characterSpriteWidth;

	Size playingSize;
	std::vector<TrapObject*> trapObjectList;
	std::vector<ItemObject*> itemObjectList;
	std::vector<Enemy*> enemyObjectList;
};

#endif // __HELLOWORLD_SCENE_H_