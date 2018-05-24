#include "HelloWorldScene.h"
#include "Character.h"

#define MAGNET_DURATION 15.0f
#define SHIELD_DURATION 10.0f
#define INVUL_DURATION 3.0f

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

	shieldSprite = Sprite::create("shield.png");
	shieldSprite->setAnchorPoint(anchor);
	shieldSprite->setPosition(x, y);
	shieldSprite->setName("Player_Shield");
	shieldSprite->setOpacity(150);
	shieldSprite->setRotation(-90);
	shieldSprite->setVisible(false);

	eDir = eRight;
	
	Run();

	// speed for jump movement - smaller faster, bigger slower
	fSpeed = 0.001f;

	fScore = 0.0f;
	fDistance = 0.0f;
	eStat = eRun;
	magnetActive = false;
	magnetTimer = 0.0f;
	magnetDuration = MAGNET_DURATION;
	lifeCount = 3;
	shieldActive = false;
	shieldTimer = 0.0f;
	shieldDuration = SHIELD_DURATION;
	invulActive = false;
	invulTimer = 0.0f;
	invulDuration = INVUL_DURATION;
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

void GameChar::Update(float delta)
{
	if (lifeCount < 0)
		return;

	fScore += 1;
	fDistance += 1;

	// Update Power-ups
	if (magnetActive)
	{
		magnetTimer += 1 * delta;
		// Deactivate magnet & reset its timer
		if (magnetTimer >= magnetDuration)
		{
			magnetActive = false;
			magnetTimer = 0.0f;
		}
	}
	if (shieldActive)
	{
		shieldTimer += 1 * delta;
		// Deactivate shield & reset its timer
		if (shieldTimer >= shieldDuration)
		{
			shieldActive = false;
			shieldTimer = 0.0f;
			shieldSprite->setVisible(false);
		}
	}
	if (invulActive)
	{
		invulTimer += 1 * delta;
		// Deactivate invulnability & reset its timer
		if (invulTimer >= invulDuration)
		{
			invulActive = false;
			invulTimer = 0.0f;
		}
	}
}

void GameChar::Run()
{
	mainSprite->stopAllActions();

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(7);

	// Right
	if (eDir > 0)
	{
		// Rotate anti-clockwise by 90(For Right side)
		mainSprite->setRotation(-90);
		if (shieldActive)
			shieldSprite->setRotation(-90);
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
		// Rotate clockwise by 90(For Left side)
		mainSprite->setRotation(90);
		if (shieldActive)
			shieldSprite->setRotation(90);
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

// Jump to TargetX
void GameChar::Jump(float LTargetX, float RTargetX, float height)
{
	mainSprite->stopAllActions();

	// Reset rotation for jump sprites
	mainSprite->setRotation(0);
	float TargetY = mainSprite->getPosition().y;
	Vec2 vec = Vec2(0, TargetY);
	float duration = Vec2(RTargetX, TargetY).length() * fSpeed;

	// Currently on Right side, Target Left side
	if (eDir > 0)
	{
		vec.x = LTargetX;
	}
	// Currently on Left side, Target Right side
	else
	{
		vec.x = RTargetX;
	}
	auto jumpEvent = JumpTo::create(duration, vec, height, 1);

	// Animation frames
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(5);

	// Right
	if (eDir > 0)
	{
		animFrames.pushBack(SpriteFrame::create("jump_left_01.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_left_02.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_left_03.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_left_04.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_left_05.png", Rect(0, 0, 59, 88)));
	}
	// Left
	else
	{
		animFrames.pushBack(SpriteFrame::create("jump_right_01.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_right_02.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_right_03.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_right_04.png", Rect(0, 0, 59, 88)));
		animFrames.pushBack(SpriteFrame::create("jump_right_05.png", Rect(0, 0, 59, 88)));
	}
	
	// Create animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animateJump = Animate::create(animation);
	
	// Run Animation
	mainSprite->runAction(animateJump);
	shieldSprite->setRotation(0);

	// Set Direction to the opposite side after jumping
	eDir = (eDir > 0) ? eLeft : eRight;

	// Function callback to stop Character jumping animation
	auto callbackRun = CallFunc::create([]()
	{
		auto scene = Director::getInstance()->getRunningScene();
		HelloWorld* HWScene = dynamic_cast<HelloWorld*>(scene);
		if (HWScene != NULL)
		{
			HWScene->getChar()->Run(); // Call run function to change animation back to run
			HWScene->getChar()->setStatus(eRun);
		}
	});

	// Setting sequence of actions to jump
	auto seq = Sequence::create(jumpEvent, callbackRun, nullptr);
	mainSprite->runAction(seq);
}
