#include "HelloWorldScene.h"
#include "Character.h"

GameChar::GameChar()
{
	mainSprite = nullptr;
}

GameChar::~GameChar()
{
}

void GameChar::init(const char * filename, Vec2 anchor, float x, float y, const char * name)
{
	mainSprite = Sprite::create(filename);
	mainSprite->setAnchorPoint(anchor);
	mainSprite->setPosition(x, y);
	mainSprite->setName(name);

	// Rotate anti-clockwise by 90(For Right side)
	mainSprite->setRotation(-90);
	eDir = eRight;
	
	Run();
	// speed for mouse click movement - smaller faster, bigger slower
	//fSpeed = 0.001f;

}

// MoveChar

//void GameChar::MoveChar(EAction dir)
//{
//	eDir = dir;
//	mainSprite->stopAllActions();
//
//	if (eDir != eStop)
//	{
//		Vector<SpriteFrame*> animFrames;
//		animFrames.reserve(7);
//
//		// Right
//		if (eDir > 0)
//		{
//			animFrames.pushBack(SpriteFrame::create("run_right_01.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_02.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_03.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_04.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_05.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_06.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_right_07.png", Rect(0, 0, 57, 85)));
//		}
//		else // Left
//		{
//			animFrames.pushBack(SpriteFrame::create("run_left_01.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_02.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_03.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_04.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_05.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_06.png", Rect(0, 0, 57, 85)));
//			animFrames.pushBack(SpriteFrame::create("run_left_07.png", Rect(0, 0, 57, 85)));
//		}
//
//		// Create animation out of the frames
//		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
//		Animate* animateMove = Animate::create(animation);
//
//		// Run Animation
//		mainSprite->runAction(RepeatForever::create(animateMove));
//	}
//	else // Idle
//	{
//		Stop();
//	}
//}

// MoveCharByCoord

//void GameChar::MoveCharByCoord(float fX, float fY)
//{
//	mainSprite->stopAllActions();
//	float diffX = fX - mainSprite->getPosition().x;
//	float diffY = fY - mainSprite->getPosition().y;
//	Vec2 vec = Vec2(diffX, diffY);
//	auto moveEvent = MoveBy::create(vec.length() * fSpeed, vec);
//
//	// Function callback to stop Character moving animation
//	auto callbackStop = CallFunc::create([]()
//	{
//		auto scene = Director::getInstance()->getRunningScene();
//		HelloWorld* HWScene = dynamic_cast<HelloWorld*>(scene);
//		if (HWScene != NULL)
//			HWScene->getChar()->Stop(); // Call stop function to change animation back to idle
//
//	});
//
//	// Setting sequence of actions to run
//	auto seq = Sequence::create(moveEvent, callbackStop, nullptr);
//	mainSprite->runAction(seq);
//
//	// Animation frames
//	Vector<SpriteFrame*> animFrames;
//	animFrames.reserve(4);
//	animFrames.pushBack(SpriteFrame::create("Blue_Back2.png", Rect(0, 0, 64, 81)));
//	animFrames.pushBack(SpriteFrame::create("Blue_Back1.png", Rect(0, 0, 65, 82)));
//	animFrames.pushBack(SpriteFrame::create("Blue_Back3.png", Rect(0, 0, 65, 81)));
//	animFrames.pushBack(SpriteFrame::create("Blue_Back1.png", Rect(0, 0, 65, 82)));
//
//	// Create animation out of the frames
//	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
//	Animate* animateMove = Animate::create(animation);
//
//	// Run Animation
//	mainSprite->runAction(animateMove);
//}

void GameChar::Update(float delta){}

void GameChar::Run()
{
	mainSprite->stopAllActions();

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(7);

	// Right
	if (eDir > 0)
	{
		animFrames.pushBack(SpriteFrame::create("run_right_01.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_02.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_03.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_04.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_05.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_06.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_right_07.png", Rect(0, 0, 57, 85)));
	}
	else // Left
	{
		animFrames.pushBack(SpriteFrame::create("run_left_01.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_02.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_03.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_04.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_05.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_06.png", Rect(0, 0, 57, 85)));
		animFrames.pushBack(SpriteFrame::create("run_left_07.png", Rect(0, 0, 57, 85)));
	}

	// Create animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animateIdle = Animate::create(animation);

	// Run Animation and repeat it forever
	mainSprite->runAction(RepeatForever::create(animateIdle));
}
