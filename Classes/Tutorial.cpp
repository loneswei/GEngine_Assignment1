#include "Tutorial.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"
//#include "Character.h"
#include "HelloWorldScene.h"
USING_NS_CC;

//#define WALL_MOVESPEED 250
//#define WALL_CONTENTSIZE_X 50
#define PLAY_POSITION 0.5
#define HIGHSCORE_POSITION 0.35
#define TUTORIAL_POSITION 0.2
Scene* Tutorial::createScene()
{
	// return scene
	return Tutorial::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Tutorial::init()
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
	//wallObjects = Node::create();
	//wallObjects->setName("WallObjects");

	//auto wallSprite_LEFT = Sprite::create("woodwall.jpg");
	//auto wallSprite_LEFT02 = Sprite::createWithTexture(wallSprite_LEFT->getTexture());
	//auto wallSprite_RIGHT = Sprite::createWithTexture(wallSprite_LEFT->getTexture());
	//auto wallSprite_RIGHT02 = Sprite::createWithTexture(wallSprite_LEFT->getTexture());

	//wallSprite_LEFT->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height));
	//wallSprite_LEFT02->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height + playingSize.height * 0.25f));
	//wallSprite_RIGHT->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height));
	//wallSprite_RIGHT02->setContentSize(Size(WALL_CONTENTSIZE_X, playingSize.height + playingSize.height * 0.25f));

	//wallSprite_LEFT->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//wallSprite_LEFT->setPosition(Vec2(0.f, playingSize.height * 0.5f));

	//wallSprite_LEFT02->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//wallSprite_LEFT02->setPosition(Vec2(0, (playingSize.height * 0.5f) + playingSize.height));

	//wallSprite_RIGHT->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//wallSprite_RIGHT->setPosition(Vec2(playingSize.width, playingSize.height * 0.5f));

	//wallSprite_RIGHT02->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//wallSprite_RIGHT02->setPosition(Vec2(playingSize.width, (playingSize.height * 0.5f) + playingSize.height));

	//wallObjects->addChild(wallSprite_LEFT, 0);
	//wallObjects->addChild(wallSprite_LEFT02, 0);
	//wallObjects->addChild(wallSprite_RIGHT, 0);
	//wallObjects->addChild(wallSprite_RIGHT02, 0);
	//this->addChild(wallObjects, 1);

	/*wallSprite_LEFT->runAction(RepeatForever::create(MoveTo::create(8, Vec2(0, -playingSize.height))));
	wallSprite_LEFT02->runAction(RepeatForever::create(MoveTo::create(14, Vec2(0, -playingSize.height))));*/
	/*wallSprite_RIGHT->runAction(wallMovementRight);
	wallSprite_RIGHT02->runAction(wallMovementRight);*/

	//wallObjects->runAction(wallMovement);

	/*
	***********************
	***** Player Init *****
	***********************
	*/
 
     ninja = Sprite::create("run_right_01.png");
	 ninja->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 ninja->setPosition(Vec2(playingSize.width * 0.05f, playingSize.height * 0.8f));
	 this->addChild(ninja, 1);

	 Samurai = Sprite::create("samurai_run_right_01.png");
	 Samurai->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 Samurai->setPosition(Vec2(playingSize.width * 0.05f, playingSize.height * 0.6f));
	 this->addChild(Samurai, 1);

	 Trap = Sprite::create("spiketrap.png");
	 Trap->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 Trap->setPosition(Vec2(playingSize.width * 0.15f, playingSize.height * 0.6f));
	 this->addChild(Trap, 1);

	 shuriken = Sprite::create("shuriken.png");
	 shuriken->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 shuriken->setPosition(Vec2(playingSize.width * 0.1f, playingSize.height * 0.6f));
	 this->addChild(shuriken, 1);

	 Shield = Sprite::create("shield.png");
	 Shield->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 Shield->setPosition(Vec2(playingSize.width * 0.05f, playingSize.height * 0.45f));
	 this->addChild(Shield, 1);


	 Magnet = Sprite::create("magnet.png");
	 Magnet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 Magnet->setPosition(Vec2(playingSize.width * 0.05f, playingSize.height * 0.3f));
	 this->addChild(Magnet, 1);

	 Coin = Sprite::create("coin.png");
	 Coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 Coin->setPosition(Vec2(playingSize.width * 0.05f, playingSize.height * 0.15f));
	 this->addChild(Coin, 1);

	 goback = Label::createWithTTF("Press Backspace to return", "fonts/Marker Felt.ttf", 24);
	 goback->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.05f));
	 this->addChild(goback, 1);


	 Title = Label::createWithTTF("TUTORIAL", "fonts/Marker Felt.ttf", 64);
	 Title->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.95f));
	 this->addChild(Title, 1);

	 shielddes = Label::createWithTTF("Grants you invulnerability for 10 seconds", "fonts/Marker Felt.ttf", 24);
	 shielddes->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.45f));
	 this->addChild(shielddes, 1);

	 Magnetdes = Label::createWithTTF("Automatically absorbs coins for 15 seconds!", "fonts/Marker Felt.ttf", 24);
	 Magnetdes ->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.3f));
	 this->addChild(Magnetdes, 1);

	 Coindes = Label::createWithTTF("Gives you extra points!", "fonts/Marker Felt.ttf", 24);
	 Coindes->setPosition(Vec2(playingSize.width * 0.35f, playingSize.height * 0.15f));
	 this->addChild(Coindes, 1);

	 ninjaplayer = Label::createWithTTF("This is you, you have 3 lifes. press Spacebar to jump left or right", "fonts/Marker Felt.ttf", 24);
	 ninjaplayer->setPosition(Vec2(playingSize.width * 0.4f, playingSize.height * 0.8f));
	 this->addChild(ninjaplayer, 1);

	 Trapshruiken = Label::createWithTTF("They are dangerous. Avoid them at all cost, they make you lose a life.", "fonts/Marker Felt.ttf", 24);
	 Trapshruiken->setPosition(Vec2(playingSize.width * 0.5f, playingSize.height * 0.6f));
	 this->addChild(Trapshruiken, 1);

	 back = ui::Button::create("back.png");
	 back->setTitleText("Button Text");
	 back->setPosition(Vec2(playingSize.width * 0.9f, playingSize.height * 0.95));
	 back->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	 { switch (type)
	 {
	 case ui::Widget::TouchEventType::BEGAN:
		 break;
	 case ui::Widget::TouchEventType::ENDED:
		 //check if character is dead here
		 auto scene = MainMenu::createScene();

		 Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		 break;
		 /*default:
		 break; */
	 } });
	 back->setVisible(true);
	 this->addChild(back, 1);


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
	listener2->onKeyReleased = CC_CALLBACK_2(Tutorial::onKeyReleased, this);
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


void Tutorial::menuCloseCallback(Ref* pSender)
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

void Tutorial::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
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

void Tutorial::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	//mainChar.MoveChar(eStop);

	// Debug Code - Exit application
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		//jump to gameplay scene here
		auto scene = MainMenu::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
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

void Tutorial::onMouseUp(Event * event)
{
	//EventMouse* e = (EventMouse*)event;
	//float x = e->getCursorX();
	//float y = e->getCursorY();

	//mainChar.MoveCharByCoord(x, y);
}

void Tutorial::update(float delta)
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

GameChar* Tutorial::getChar()
{
	return &mainChar;
}
