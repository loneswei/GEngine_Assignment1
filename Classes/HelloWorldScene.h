#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "TrapObjects.h"

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

	//Fetch a free trap
	TrapObject* FetchTrapObject(TrapObject::TRAP_TYPE trapType = TrapObject::TRAP_RANDOM);

	// Get Char
	GameChar* getChar();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	GameChar mainChar;
	Node *wallObjects;
	Label* scoreLabel;
	Label* distanceLabel;

	Size playingSize;
	std::vector<TrapObject*> trapObjectList;
};

#endif // __HELLOWORLD_SCENE_H_
