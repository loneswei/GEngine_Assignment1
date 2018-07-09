#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "Tutorial.h"
//#include "Character.h"
#include "HelloWorldScene.h"
#include "ui\CocosGUI.h"
USING_NS_CC;

//#define WALL_MOVESPEED 250
//#define WALL_CONTENTSIZE_X 50
#define PLAY_POSITION 0.5
#define HIGHSCORE_POSITION 0.35
#define TUTORIAL_POSITION 0.2
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

	//Create the walls that player runs on
 

	/*
	***********************
	***** Player Init *****
	***********************
	*/

	/*background = Sprite::create("mainmenubackground.jpg");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.5f));
	this->addChild(background, 1);
*/
	GameTitle = Label::createWithTTF("HELLOTHERE", "fonts/Marker Felt.ttf", 50);
	GameTitle->enableShadow();
	GameTitle->enableOutline(Color4B::BLACK, 1);
	GameTitle->enableGlow(Color4B::YELLOW);
	GameTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	GameTitle->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 10));
	this->addChild(GameTitle, 1);

	 //testing button here
	auto button = ui::Button::create("Button_Normal.png", "Button_Press.png", "Button_Disable.png");
	button->setTitleText("Button Text");
	button->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 5));
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{ switch (type) 
	{ case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		//std::cout << "Button 1 clicked" << std::endl; 
		break; 
	default:
		break; 
	} }); 
	this->addChild(button);
	//testing checkbox here
	auto checkbox = ui::CheckBox::create("CheckBoxNode_Disable.png", "CheckBoxNode_Normal.png","CheckBox_Normal.png","CheckBox_Press.png","CheckBox_Disable.png");
	 
	checkbox->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 5));
	 
	checkbox->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) 
	{ switch (type) 
	{ case ui::Widget::TouchEventType::BEGAN:
		break; 
	case ui::Widget::TouchEventType::ENDED:
		 break; 
	default:
		break; 
	}});
	this->addChild(checkbox);

	auto slider = ui::Slider::create(); slider->loadBarTexture("Slider_Back.png"); 
	slider->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 5));
	slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
	slider->loadProgressBarTexture("Slider_PressBar.png"); 
	slider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) { 
		switch (type) { 
		case ui::Widget::TouchEventType::BEGAN:
			break; 
		case ui::Widget::TouchEventType::ENDED:
			//std::cout << "slider moved" << std::endl;
			break; 
		default: 
			break;
		}}); 
	this->addChild(slider);

	auto textField = ui::TextField::create("", "Arial", 30); 
	textField->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 2));
	textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) { 
		//std::cout << "editing a TextField" << std::endl; 
	}); 
	textField->setPasswordEnabled(true);
	textField->setMaxLength(10);
	this->addChild(textField);
	 

    Play = Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 24);
	Play->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 7));
	this->addChild(Play, 1);

	 

	Tutorial = Label::createWithTTF("TUTORIAL", "fonts/Marker Felt.ttf", 24);
	Tutorial->setPosition(Vec2(playingSize.width * 0.5f, 0 + GameTitle->getContentSize().height * 3));
	this->addChild(Tutorial, 1);

	//auto playerObject = Node::create();
	//playerObject->setName("PlayerObject");

	//auto playerSprite = Sprite::create("run_right_01.png");

	//mainChar.init("run_right_01.png", Vec2::ANCHOR_MIDDLE_BOTTOM, (playingSize.width - (WALL_CONTENTSIZE_X * 0.5f)), (playerSprite->getContentSize().width * 2), "Player");
	//playerObject->addChild(mainChar.getSprite(), 1);
	//this->addChild(playerObject, 1);

	//scoreLabel = Label::createWithTTF("Score: " + std::to_string(mainChar.getScore()), "fonts/Marker Felt.ttf", 24);
	//scoreLabel->setPosition(Vec2(playingSize.width * 0.5f, 0 + scoreLabel->getContentSize().height));
	//this->addChild(scoreLabel, 1);


	////auto PlayerHealthNode = Node::create();

	//auto PlayerHealth = Sprite::create("player_life.jpg");
	//PlayerHealth->setPosition(Vec2(0, PlayerHealth->getContentSize().height));
	//this->addChild(PlayerHealth, 1);



	//// Practical 01
	//auto sprite = Sprite::create("ZigzagGrass_Mud_Round.png");

	//sprite->setAnchorPoint(Vec2::ZERO);
	//sprite->setPosition(0, (visibleSize.height / 2) - (sprite->getContentSize().height / 2));
	//
	//nodeItems->addChild(sprite, 0);
	//this->addChild(nodeItems, 1);

	// Platform
	/*int limit = std::ceil(visibleSize.width / sprite->getContentSize().width);

	for (int i = 1; i < limit; i++)
	{
	auto spriteT = Sprite::createWithTexture(sprite->getTexture());

	spriteT->setAnchorPoint(Vec2::ZERO);
	spriteT->setPosition(i * spriteT->getContentSize().width, (visibleSize.height / 2) - (spriteT->getContentSize().height / 2));

	nodeItems->addChild(spriteT, 0);
	}*/

	// Character
	 
	/*auto spriteNode = Node::create();
	spriteNode->setName("ArrowChoose");*/

	arrow = Sprite::create("arrow.png");
	arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	arrow->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.5f));
	this->addChild(arrow, 1);
	selection = PLAY;
	
 

	/*mainChar.init("Blue_Front1.png", Vec2::ZERO, 0, (visibleSize.height / 2) + (sprite->getContentSize().height / 2), "mainSprite");
	spriteNode->addChild(mainChar.getSprite(), 1);
	this->addChild(spriteNode, 1);*/

	//// auto move to x:200
	//auto moveEvent = MoveBy::create(1, Vec2(200, 0));
	//mainSprite->runAction(moveEvent);

	//// auto reverse back to x:0
	//auto delay = DelayTime::create(5.0f);
	//auto delaySequence = Sequence::create(delay, delay->clone(), nullptr);
	//auto sequence = Sequence::create(moveEvent, moveEvent->reverse(), delaySequence, nullptr);
	//mainSprite->runAction(sequence);

	// Practical 02
	// Key Pressed movement
	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//// Key Released movement
	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	//// Mouse button Up
	//auto listener3 = EventListenerMouse::create();
	//listener3->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);

	// Call Update function
	this->scheduleUpdate();

	// Practical 03
	// Load Idle animation frames
	//Vector<SpriteFrame*> animFrames;
	//animFrames.reserve(4);
	//animFrames.pushBack(SpriteFrame::create("Blue_Front2.png", Rect(0, 0, 65, 81)));
	//animFrames.pushBack(SpriteFrame::create("Blue_Front1.png", Rect(0, 0, 65, 81)));
	//animFrames.pushBack(SpriteFrame::create("Blue_Front3.png", Rect(0, 0, 65, 81)));
	//animFrames.pushBack(SpriteFrame::create("Blue_Front1.png", Rect(0, 0, 65, 81)));

	//// Create animation out of the frames
	//Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	//Animate* animateIdle = Animate::create(animation);

	// Run Animation and repeat it forever
	//mainChar.getSprite()->runAction(RepeatForever::create(animateIdle));

	// Loading Sprite Sheet
	/*SpriteBatchNode* spritebatch = SpriteBatchNode::create("sprite.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprite.plist");*/

	// Loading sprite from Sprite Sheet
	//auto Sprite1 = Sprite::createWithSpriteFrameName("Blue_Back1.png");
	//spritebatch->addChild(Sprite1);
	//addChild(spritebatch);

	// Loading animation from Sprite Sheet
	//Vector<SpriteFrame*> animFrames(3);

	//char str[100] = { 0 };
	//for (int i = 1; i < 3; i++)
	//{
	//	sprintf(str, "Blue_Back%d.png", i);
	//	SpriteFrame* frame = cache->getSpriteFrameByName(str);
	//	animFrames.pushBack(frame);
	//}

	//// "caches" are always singletons in cocos2d
	//auto anim_cache = AnimationCache::getInstance();
	//anim_cache->addAnimationsWithFile("sprite_ani.plist");

	//// should be getAnimationByName(..) in future versions
	//auto animation = anim_cache->animationByName("walk_right");

	//// Don't confused between Animation and Animate
	//auto animate = Animate::create(animation);
	//mainChar.getSprite()->runAction(animate);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//auto closeItem = MenuItemImage::create(
	//                                       "CloseNormal.png",
	//                                       "CloseSelected.png",
	//                                       CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	//if (closeItem == nullptr ||
	//    closeItem->getContentSize().width <= 0 ||
	//    closeItem->getContentSize().height <= 0)
	//{
	//    problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	//}
	//else
	//{
	//    float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
	//    float y = origin.y + closeItem->getContentSize().height/2;
	//    closeItem->setPosition(Vec2(x,y));
	//}

	// create menu, it's an autorelease object
	//auto menu = Menu::create(closeItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	//auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	//if (label == nullptr)
	//{
	//    problemLoading("'fonts/Marker Felt.ttf'");
	//}
	//else
	//{
	//    // position the label on the center of the screen
	//    label->setPosition(Vec2(origin.x + visibleSize.width/2,
	//                            origin.y + visibleSize.height - label->getContentSize().height));

	//    // add the label as a child to this layer
	//    this->addChild(label, 1);
	//}

	// add "HelloWorld" splash screen"
	//auto sprite = Sprite::create("HelloWorld.png");
	//if (sprite == nullptr)
	//{
	//    problemLoading("'HelloWorld.png'");
	//}
	//else
	//{
	//    // position the sprite on the center of the screen
	//    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	//    // add the sprite as a child to this layer
	//    this->addChild(sprite, 0);
	//}
	return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
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

void MainMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
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

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	//mainChar.MoveChar(eStop);

	// Debug Code - Exit application
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection == PLAY)
	{
		//jump to gameplay scene here
		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2,scene));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && selection == TUTORIAL)
	{
		//jump to gameplay scene here
		auto scene = Tutorial::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		//move down here
		switch (selection)
		{
		case PLAY:
			arrow->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * TUTORIAL_POSITION));
			selection = TUTORIAL;
			break;
		case TUTORIAL:
			break;
		}

	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		//move up here & check enum
		switch (selection)
		{
		case PLAY:
			break;
	 
		case TUTORIAL:
			arrow->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * TUTORIAL_POSITION));
			selection = PLAY;
			break;
		}
	}

	//// Jump to the opposite side
	//if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	//{
	//	// Only allow Jumping when Status is eRun
	//	if (mainChar.getStatus() == eRun)
	//	{
	//		mainChar.setStatus(eJump);
	//		float LTarget = WALL_CONTENTSIZE_X * 0.5f;
	//		float RTarget = playingSize.width - (WALL_CONTENTSIZE_X * 0.5f);

	//		mainChar.Jump(LTarget, RTarget, playingSize.height / 2);
	//	}
	//}
}

void MainMenu::onMouseUp(Event * event)
{
	//EventMouse* e = (EventMouse*)event;
	//float x = e->getCursorX();
	//float y = e->getCursorY();

	//mainChar.MoveCharByCoord(x, y);
}

void MainMenu::update(float delta)
{
	//scoreLabel->setString("Score: " + std::to_string(mainChar.getScore()));
	//mainChar.Update(delta);

	
	/*static auto wallObjectsArray = wallObjects->getChildren();

	for (auto wallSprite : wallObjectsArray)
	{
		if (wallSprite->getPositionY() < -playingSize.height * 0.5f)
		{
			wallSprite->setPosition(Vec2(wallSprite->getPositionX(), (playingSize.height * 0.5f) + playingSize.height));
		}
		else
		{
			wallSprite->setPosition(Vec2(wallSprite->getPositionX(), wallSprite->getPositionY() - WALL_MOVESPEED * delta));
		}
	}*/
}

GameChar* MainMenu::getChar()
{
	return &mainChar;
}
