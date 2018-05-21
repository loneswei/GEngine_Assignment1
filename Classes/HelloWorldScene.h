#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "TrapObjects.h"
#include "Enemy.h"
#include "ItemObjects.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onMouseUp(Event* event);

	// Update funciton that cals every frame
	virtual void update(float delta);

	//Fetchers
	TrapObject* FetchTrapObject(TrapObject::TRAP_TYPE trapType = TrapObject::TRAP_RANDOM);
	Enemy* FetchEnemyObject(Enemy::ENEMY_TYPE enemyType = Enemy::ENEMY_RANDOM);

	// Get Char
	GameChar* getChar();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	GameChar mainChar;
	Node *wallObjects;
	Node *trapObjects;
	Node *enemies;
	Node *itemObjects;

	Label* scoreLabel;
	Label* distanceLabel;

	Size playingSize;
	std::vector<TrapObject*> trapObjectList;
	std::vector<Enemy*> enemyList;
	std::vector<ItemObject*> itemObjectList;
};

#endif // __HELLOWORLD_SCENE_H_
