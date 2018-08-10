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
#define ELEMENT_WIDTH 200
#define ELEMENT_HEIGHT 200
Vec2 StartingPos(300.0f, 800.0f);

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

	//-------Shop tabs-------//
	SkinTabOpened = false;
	SkinTab = ui::Button::create("Skins/Default/jump_right_01.png");
	SkinTab->setTitleText("SKIN");
	SkinTab->setTitleFontSize(2.5f * SkinTab->getTitleFontSize());
	SkinTab->getTitleLabel()->setPositionY(SkinTab->getTitleLabel()->getPositionY() + (0.6f * SkinTab->getContentSize().height));
	SkinTab->setPosition(Vec2(playingSize.width * 0.1f, 0 + playingSize.height * 0.8f));
	SkinTab->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//Button clicked
		ExitPowerup();
		InitSkin();
		break;
	} });

	SkinTab->setVisible(false);
	this->addChild(SkinTab);

	PowerupTabOpened = false;
	PowerupTab = ui::Button::create("coin.png");
	PowerupTab->setTitleText("POWER UP");
	PowerupTab->setTitleFontSize(2.5f * PowerupTab->getTitleFontSize());
	PowerupTab->getTitleLabel()->setPositionY(PowerupTab->getTitleLabel()->getPositionY() + (0.6f * PowerupTab->getContentSize().height));
	PowerupTab->setPosition(Vec2(playingSize.width * 0.1f, 0 + playingSize.height * 0.6f));
	PowerupTab->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//Button clicked
		ExitSkin();
		InitPowerup();
		break;
	} });

	PowerupTab->setVisible(false);
	this->addChild(PowerupTab);
	//-----------------------//

	//// Key Released movement
	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	//touchListener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
	//	if (SkinTabOpened)
	//	{
	//		for (auto *skin : SkinElements)
	//		{
	//			float buybuttonHalfWidth = skin->BuyButton->getContentSize().width * skin->BuyButton->getScaleX() * 0.5f, buybuttonHalfHeight = skin->BuyButton->getContentSize().height * skin->BuyButton->getScaleY() * 0.5f;

	//			if (touch->getLocation().x > skin->BuyButton->getPositionX() - buybuttonHalfWidth && touch->getLocation().x < skin->BuyButton->getPositionX() + buybuttonHalfWidth &&
	//				touch->getLocation().y > skin->BuyButton->getPositionY() - buybuttonHalfHeight && touch->getLocation().y < skin->BuyButton->getPositionY() + buybuttonHalfHeight)
	//			{
	//				if (skin->isEquipped)
	//				{
	//					break;
	//				}
	//				else if (skin->isBought)
	//				{
	//					//Uneqip previously equipped skin
	//					for (auto *skin : SkinElements)
	//					{
	//						if (skin->GetName() == SaveData::GetInstance().EquippedSkinName)
	//						{
	//							skin->BuyButtonLabel->setString("EQUIP");
	//						}
	//					}

	//					//Update the current equipped skin
	//					skin->BuyButtonLabel->setString("EQUIPPED");
	//					SaveData::GetInstance().EquippedSkinName = skin->GetName();

	//					break;
	//				}
	//				else
	//				{
	//					//Update it as being bought
	//					skin->BuyButtonLabel->setString("BOUGHT");
	//					SaveData::GetInstance().BoughtSkins.push_back(skin->GetName());

	//					//Update gold amount
	//					PlayerGold -= skin->GetPrice();
	//					break;
	//				}

	//			}
	//		}
	//	}

	//	return true;
	//};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Call Update function
	this->scheduleUpdate();

	return true;
}

void MainMenu::InitShop()
{
	//Create the nodes
	SkinNode = Node::create();
	PowerupNode = Node::create();
	
	//Default tab: Skin
	InitSkin();

	//Parent nodes to scene
	this->addChild(SkinNode, 1);
	this->addChild(PowerupNode, 1);
	//this->addChild(BuyButtonNode, 1);
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
	//Show Tabs
	SkinTab->setVisible(true);
	PowerupTab->setVisible(true);

	PlayerGold = SaveData::GetInstance().GetGold();
}

void MainMenu::ShopToMainMenu()
{
	background->setVisible(true);
	tutorialbutton->setVisible(true);
	shopbutton->setVisible(true);
	playbutton->setVisible(true);
	GameTitle->setVisible(true);
	backbutton->setVisible(false);
	//Hide Tabs
	SkinTab->setVisible(false);
	PowerupTab->setVisible(false);

	SaveData::GetInstance().SetGold(PlayerGold);
}

void MainMenu::InitSkin()
{
	if (!SkinTabOpened)
	{
		//Add Skins
		AddSkin("Default", "Skins/Default/jump_right_01.png");
		AddSkin("Blue", "Skins/Blue/jump_right_01.png");
		AddSkin("Green", "Skins/Green/jump_right_01.png");
		AddSkin("Grey", "Skins/Grey/jump_right_01.png");
		AddSkin("Red", "Skins/Red/jump_right_01.png");

		//Adjust skin elements positions
		unsigned short Col = 1, Row = 0;

		//Add Skin elements to shop screen
		for (size_t i = 0; i < SkinElements.size(); ++i)
		{
			auto *theSkinElement = SkinElements[i];

			if (Col == 1)
			{
				theSkinElement->SetPosition(StartingPos.x, StartingPos.y - (Row * ELEMENT_HEIGHT));
			}
			else
			{
				Vec2 prevElementPos = SkinElements[i - 1]->GetPosition();
				theSkinElement->SetPosition(prevElementPos.x + ELEMENT_WIDTH, prevElementPos.y);
			}

			//Determin if skin is equipped
			if (theSkinElement->GetName() == SaveData::GetInstance().EquippedSkinName)
			{
				theSkinElement->isEquipped = true;
			}

			//Determine if skin is bought
			theSkinElement->isBought = (std::find(SaveData::GetInstance().BoughtSkins.begin(), SaveData::GetInstance().BoughtSkins.end(), theSkinElement->GetName()) != SaveData::GetInstance().BoughtSkins.end());

			++Col;

			if (Col == COLUMN_NUMBER)
			{
				Col = 1;
				++Row;
			}
		}

		//Add buy button under each element
		for (size_t i = 0; i < SkinElements.size(); ++i)
		{
			auto BuyButton = ui::Button::create("");
			auto *theElement = SkinElements[i];
			
			if (theElement->isEquipped)
			{
				BuyButton->setTitleText("EQUIPPED");
			}
			else if (theElement->isBought)
			{
				BuyButton->setTitleText("EQUIP");
			}
			else
			{
				BuyButton->setTitleText("BUY");
			}
			BuyButton->setName(std::to_string(i));
			BuyButton->setTitleFontSize(2.5f * BuyButton->getTitleFontSize());
			BuyButton->setPosition(Vec2(SkinElements[i]->GetPosition().x, SkinElements[i]->GetPosition().y - (ELEMENT_HEIGHT * 0.35f)));
			BuyButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					break;
				case ui::Widget::TouchEventType::ENDED:
					//Button clicked
					
					ui::Button *BuyButton = nullptr;

					for (size_t i = 0; i < BuyButtonList.size(); ++i)
					{
						if (BuyButtonList[i]->_ID == sender->_ID)
						{
							BuyButton = BuyButtonList[i];
							break;
						}
					}
					auto *theElement = SkinElements[std::stoi(BuyButton->getName())];

					if (theElement->isEquipped)
					{
						break;
					}
					else if (theElement->isBought)
					{
						//Uneqip previously equipped skin
						for (auto *button : BuyButtonList)
						{
							if (button->getTitleText() == "EQUIPPED")
							{
								button->setTitleText("EQUIP");
								break;
							}
						}

						for (auto *element : SkinElements)
						{
							if (element->isEquipped)
							{
								element->isEquipped = false;
								break;
							}
						}

						//Update the current button as equipped
						BuyButton->setTitleText("EQUIPPED");

						//Update the current equipped skin
						SaveData::GetInstance().EquippedSkinName = theElement->GetName();
						theElement->isEquipped = true;

						break;
					}
					else
					{
						//Update it as being bought
						BuyButton->setTitleText("EQUIP");
						SaveData::GetInstance().BoughtSkins.push_back(theElement->GetName());
						theElement->isBought = true;

						//Update gold amount
						PlayerGold -= theElement->GetPrice();
						break;
					}

					break;
				}
			});

			BuyButton->setVisible(true);
			this->addChild(BuyButton);
			BuyButtonList.push_back(BuyButton);
		}

		SkinTabOpened = true;
	}
}

void MainMenu::InitPowerup()
{
	if (!PowerupTabOpened)
	{
		//Add Powerups
		AddPowerup("Coin", "coin.png");
		AddPowerup("Magnet", "magnet.png");
		AddPowerup("Shield", "shield.png");
		AddPowerup("Shuriken", "shuriken.png");
		AddPowerup("Spike", "spiketrap.png");

		//Adjust powerup elements positions
		unsigned short Col = 1, Row = 0;

		for (size_t i = 0; i < PowerupElements.size(); ++i)
		{
			if (Col == 1)
			{
				PowerupElements[i]->SetPosition(StartingPos.x, StartingPos.y - (Row * ELEMENT_HEIGHT));
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
		PowerupTabOpened = true;
	}
}

void MainMenu::AddSkin(const std::string &Name, const std::string &SpriteFilePath, const unsigned int &Price)
{
	//Create the new skin element
	ShopElement *skin = new ShopElement(Name, SpriteFilePath, Price);
	skin->isEquipped = false;
	
	//If it's the first element, assign it the starting pos
	if (SkinElements.empty())
	{
		skin->SetPosition(StartingPos);
	}

	//Add the skin element object into SkinElements
	SkinElements.push_back(skin);
	SkinNode->addChild(skin->GetSprite(), 1);

	////IF skin not found in boughskins, add it into the hashmap
	//if (SaveData::GetInstance().BoughtSkins.find(Name) != SaveData::GetInstance().BoughtSkins.end())
	//{
	//	SaveData::GetInstance().BoughtSkins.insert(std::pair<std::string, bool>(Name, false));
	//}
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
	PowerupElements.push_back(powerup);
	PowerupNode->addChild(powerup->GetSprite(), 1);
}

void MainMenu::ExitSkin()
{
	for (auto skin : SkinElements)
	{
		// Remove from Node = Remove the sprites in game
		SkinNode->removeChild(skin->GetSprite());
		
		delete skin;
		skin = nullptr;
	}
	SkinElements.clear();
	SkinTabOpened = false;

	for (auto button : BuyButtonList)
	{
		// Remove from Node = Remove the sprites in game
		this->removeChild(button);
		button = nullptr;
	}

	BuyButtonList.clear();
}

void MainMenu::ExitPowerup()
{
	for (auto powerup : PowerupElements)
	{
		// Remove from Node = Remove the sprites in game
		PowerupNode->removeChild(powerup->GetSprite());

		delete powerup;
		powerup = nullptr;
	}
	PowerupElements.clear();
	PowerupTabOpened = false;

	for (auto button : BuyButtonList)
	{
		// Remove from Node = Remove the sprites in game
		this->removeChild(button);

		delete button;
		button = nullptr;
	}
	BuyButtonList.clear();
}

void MainMenu::ExitShop()
{
	ExitSkin();
	ExitPowerup();

	SkinTabOpened = PowerupTabOpened = false;
}
