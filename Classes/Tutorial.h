#pragma once
#ifndef _Tutorial_H
#define _Tutorial_H

#include "cocos2d.h"
#include "Character.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
//enum Selection
//{
//	PLAY = 0,
//	HIGHSCORE,
//	TUTORIAL,
//};

class Tutorial : public cocos2d::Scene
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

	// Get Char
	GameChar* getChar();

	// implement the "static create()" method manually
	CREATE_FUNC(Tutorial);
private:
	GameChar mainChar;
	Node *wallObjects;
	Size playingSize;
	Label* Title;
	Label* ninjaplayer;
	Label* shielddes;
	Label* Magnetdes;
	Label* Coindes;
	Label* Trapshruiken;
	Label* goback;
	Sprite* arrow;
	Sprite* ninja;
	Sprite* Samurai;
	Sprite* Trap;
	Sprite* Shield;
	Sprite* Coin;
	Sprite* Magnet;
	Sprite* shuriken;

	cocos2d::ui::Button* back;
//	Selection selection;
};


#endif // _Tutorial_H

