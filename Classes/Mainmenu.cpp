#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

#include "MainMenu.h"
#include "Tutorial.h"
#include "HelloWorldScene.h"
USING_NS_CC;

//#define WALL_MOVESPEED 250
//#define WALL_CONTENTSIZE_X 50
#define PLAY_POSITION 0.5
#define HIGHSCORE_POSITION 0.35
#define TUTORIAL_POSITION 0.2

//Shop elements
#define COLUMN_NUMBER 4
#define ELEMENT_WIDTH 10
#define ELEMENT_HEIGHT 10
Vec2 StartingPos;

Scene* MainMenu::createScene()
{
	// return scene
	return MainMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenu::init()
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

	/*
	***********************
	***** Player Init *****
	***********************
	*/

	background = Sprite::create("mainmenubackground.jpg");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.5f));
	background->setVisible(true);
	this->addChild(background);

	GameTitle = Label::createWithTTF("HELLOTHERE", "fonts/Marker Felt.ttf", 50);
	GameTitle->enableShadow();
	GameTitle->enableOutline(Color4B::BLACK, 1);
	GameTitle->enableGlow(Color4B::YELLOW);
	GameTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	GameTitle->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 15));
	GameTitle->setVisible(true);
	this->addChild(GameTitle, 1);

	backbutton = ui::Button::create("back.png");
	backbutton->setPosition(Vec2(playingSize.width * 0.9f, playingSize.height * 0.95));
	backbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//check if character is dead here
		ExitShop();
		ShopToMainMenu();
		Director::sharedDirector()->resume();
		break;
		/*default:
		break; */
	} });
	backbutton->setVisible(false);
	this->addChild(backbutton, 1);

	 //testing button here
	playbutton = ui::Button::create("play.png");
	playbutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 8));
	playbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		break;
		/*default:
		break; */
	} });
	playbutton->setVisible(true);
	this->addChild(playbutton);

	tutorialbutton = ui::Button::create("buttontutor.png");
	tutorialbutton->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 5));
	tutorialbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		 
		auto scene = Tutorial::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		break;
		/*default:
		break; */
	} });

	tutorialbutton->setVisible(true);
	this->addChild(tutorialbutton);

	shopbutton = ui::Button::create("shop.png");
	shopbutton->setPosition(Vec2(playingSize.width * 0.9f, 0 + playingSize.height * 0.94f));
	shopbutton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		InitShop();
		MainMenuToShop();

		break;
		/*default:
		break; */
	} });

	shopbutton->setVisible(true);
	this->addChild(shopbutton);

	//// Key Released movement
	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	// Call Update function
	this->scheduleUpdate();

	return true;
}

void MainMenu::InitShop()
{
	//Create the nodes
	SkinNode = Node::create();
	PowerupNode = Node::create();
	
	//-------Add the elements to shop-------//
	
	//Add Skins
	AddSkin("Default", "Skins/Default/jump_right_01.png");

	//Add Powerups

	//-------------------------------------//
	
	//Parent nodes to scene
	this->addChild(SkinNode, 1);
	this->addChild(PowerupNode, 1);

	//Adjust skin elements positions
	unsigned short Col = 1, Row = 0;

	for (size_t i = 1; i < SkinElements.size(); ++i)
	{
		if (Col == 1)
		{
			SkinElements[i]->SetPosition(StartingPos.x - (Row * ELEMENT_HEIGHT), StartingPos.y);
		}
		else
		{
			Vec2 prevElementPos = SkinElements[i - 1]->GetPosition();
			SkinElements[i]->SetPosition(prevElementPos.x + ELEMENT_WIDTH, prevElementPos.y);
		}
		
		++Col;

		if (Col == COLUMN_NUMBER)
		{
			Col = 1;
			++Row;
		}
	}

	//Adjust powerup elements positions
	Col = 1, Row = 0;

	for (size_t i = 1; i < PowerupElements.size(); ++i)
	{
		if (Col == 1)
		{
			PowerupElements[i]->SetPosition(StartingPos.x - (Row * ELEMENT_HEIGHT), StartingPos.y);
		}
		else
		{
			Vec2 prevElementPos = PowerupElements[i - 1]->GetPosition();
			PowerupElements[i]->SetPosition(prevElementPos.x + ELEMENT_WIDTH, prevElementPos.y);
		}

		++Col;

		if (Col == COLUMN_NUMBER)
		{
			Col = 1;
			++Row;
		}
	}
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	
}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{

}

void MainMenu::onMouseUp(Event * event)
{
}

void MainMenu::update(float delta)
{
	
}

void MainMenu::MainMenuToShop()
{
	background->setVisible(false);
	tutorialbutton->setVisible(false);
	shopbutton->setVisible(false);
	playbutton->setVisible(false);
	GameTitle->setVisible(false);
	backbutton->setVisible(true);
}

void MainMenu::ShopToMainMenu()
{
	background->setVisible(true);
	tutorialbutton->setVisible(true);
	shopbutton->setVisible(true);
	playbutton->setVisible(true);
	GameTitle->setVisible(true);
	backbutton->setVisible(false);
}

void MainMenu::AddSkin(const std::string &Name, const std::string &SpriteFilePath, const unsigned int &Price)
{
	//Create the new skin element
	ShopElement *skin = new ShopElement(Name, SpriteFilePath, Price);
	
	//If it's the first element, assign it the starting pos
	if (SkinElements.empty())
	{
		skin->SetPosition(StartingPos);
	}

	//Add the skin element object into SkinElements
	SkinElements.push_back(skin);
	SkinNode->addChild(skin->GetSprite(), 1);
}

void MainMenu::AddPowerup(const std::string & Name, const std::string &SpriteFilePath, const unsigned int & Price)
{
	//Create the new powerup element
	ShopElement *powerup = new ShopElement(Name, SpriteFilePath, Price);

	//If it's the first element, assign it the starting pos
	if (PowerupElements.empty())
	{
		powerup->SetPosition(StartingPos);
	}

	//Add the skin element object into PowerupElements
	SkinElements.push_back(powerup);
	SkinNode->addChild(powerup->GetSprite(), 1);
}

void MainMenu::ExitShop()
{
	for (auto skin : SkinElements)
	{
		delete skin;
		skin = nullptr;
	}

	for (auto powerup : PowerupElements)
	{
		delete powerup;
		powerup = nullptr;
	}

	SkinElements.clear();
	PowerupElements.clear();
}
