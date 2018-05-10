#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// return scene
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto playingSize = Size(visibleSize.width, visibleSize.height - (visibleSize.height / 8));

	auto nodeItems = Node::create();
	nodeItems->setName("nodeItems");

	// Practical 01
	auto sprite = Sprite::create("ZigzagGrass_Mud_Round.png");

	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setPosition(0, (visibleSize.height / 2) - (sprite->getContentSize().height / 2));
	
	nodeItems->addChild(sprite, 0);
	this->addChild(nodeItems, 1);

	// Platform
	int limit = std::ceil(visibleSize.width / sprite->getContentSize().width);

	for (int i = 1; i < limit; i++)
	{
		auto spriteT = Sprite::createWithTexture(sprite->getTexture());

		spriteT->setAnchorPoint(Vec2::ZERO);
		spriteT->setPosition(i * spriteT->getContentSize().width, (visibleSize.height / 2) - (spriteT->getContentSize().height / 2));

		nodeItems->addChild(spriteT, 0);
	}

	// Character
	auto spriteNode = Node::create();
	spriteNode->setName("spriteNode");

	mainChar.init("Blue_Front1.png", Vec2::ZERO, 0, (visibleSize.height / 2) + (sprite->getContentSize().height / 2), "mainSprite");
	spriteNode->addChild(mainChar.getSprite(), 1);
	this->addChild(spriteNode, 1);

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
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Key Released movement
	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	// Mouse button Up
	auto listener3 = EventListenerMouse::create();
	listener3->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);

	// Call Update function
	this->scheduleUpdate();

	// Practical 03
	// Load Idle animation frames
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(4);
	animFrames.pushBack(SpriteFrame::create("Blue_Front2.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("Blue_Front1.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("Blue_Front3.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("Blue_Front1.png", Rect(0, 0, 65, 81)));

	// Create animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animateIdle = Animate::create(animation);

	// Run Animation and repeat it forever
	mainChar.getSprite()->runAction(RepeatForever::create(animateIdle));

	// Loading Sprite Sheet
	SpriteBatchNode* spritebatch = SpriteBatchNode::create("sprite.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprite.plist");

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


void HelloWorld::menuCloseCallback(Ref* pSender)
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

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	// Move to the right
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		mainChar.MoveChar(eRight);
	}
	// Move to the left
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		mainChar.MoveChar(eLeft);
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	mainChar.MoveChar(eStop);
}

void HelloWorld::onMouseUp(Event * event)
{
	EventMouse* e = (EventMouse*)event;
	float x = e->getCursorX();
	float y = e->getCursorY();

	mainChar.MoveCharByCoord(x, y);
}

void HelloWorld::update(float delta)
{
	mainChar.Update(delta);
}

GameChar* HelloWorld::getChar()
{
	return &mainChar;
}
