#pragma once
#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "cocos2d.h"
#include "Character.h"
#include "ui\CocosGUI.h"

using namespace cocos2d;

class MainMenu : public cocos2d::Scene
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

	void MainMenuToShop();
	void ShopToMainMenu();

	// Get Char
	GameChar* getChar();

	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);
private:
	
	Size playingSize;
	Label* GameTitle;
	Label* Play;
	Sprite* background;

	cocos2d::ui::Button* playbutton;
	cocos2d::ui::Button* tutorialbutton;
	cocos2d::ui::Button* shopbutton;
	cocos2d::ui::Button* backbutton;
};
#endif // _MAINMENU_H
