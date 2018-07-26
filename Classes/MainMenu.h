#pragma once
#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "cocos2d.h"
#include "ui\CocosGUI.h"

#include "Character.h"
#include "ShopElements.h"

using namespace cocos2d;

class MainMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void InitShop();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onMouseUp(Event* event);

	// Update funciton that cals every frame
	virtual void update(float delta);

	void MainMenuToShop();
	void ShopToMainMenu();

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
	cocos2d::ui::Button* SkinTab;
	cocos2d::ui::Button* PowerupTab;

	bool SkinTabOpened;
	bool PowerupTabOpened;

	std::vector<ShopElement*> SkinElements;
	std::vector<ShopElement*> PowerupElements;

	Node *SkinNode;
	Node *PowerupNode;

	void InitSkin();
	void InitPowerup();

	void AddSkin(const std::string &Name, const std::string &SpriteFilePath, const unsigned int &Price = 0);
	void AddPowerup(const std::string &Name, const std::string &SpriteFilePath, const unsigned int &Price = 0);

	void ExitSkin();
	void ExitPowerup();
	void ExitShop();
};
#endif // _MAINMENU_H
