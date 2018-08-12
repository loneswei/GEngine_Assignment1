#include "HelloWorldScene.h"
#include "Enemy2.h"

#define MAGNET_DURATION 7.0f
#define SHIELD_DURATION 7.0f
#define INVUL_DURATION 3.0f

GameEnemy::GameEnemy()
{
	mainSprite = nullptr;
}

GameEnemy::~GameEnemy()
{
}

void GameEnemy::init(const char * filename, Vec2 anchor, float x, float y, const char * name)
{
	mainSprite = Sprite::create(filename);
	mainSprite->setAnchorPoint(anchor);
	mainSprite->setPosition(x, y);
	mainSprite->setName(name);

	eDir = eRight2;

//	EquippedSkinName = SaveData::GetInstance().EquippedSkinName;
	Run();

	// speed for jump movement - smaller faster, bigger slower
	fSpeed = 0.001f;

	fScore = 0.0f;
	fDistance = 0.0f;
	eStat = eRun2;
	dead = false;

}

void GameEnemy::Update(float delta)
{


	fScore += 1;
	fDistance += 1;

	// Update Power-ups
	 
}

void GameEnemy::Run()
{
	mainSprite->stopAllActions();

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(7);

	// Right
	if (eDir == eRight2)
	{
		mainSprite->setFlippedX(false);
		// Rotate anti-clockwise by 90(For Right side)
		mainSprite->setRotation(-90);
	}
	// Left
	else if (eDir == eLeft2)
	{
		mainSprite->setFlippedX(true);
		// Rotate clockwise by 90(For Left side)
		mainSprite->setRotation(90);
	}
	mainSprite->setFlippedY(true);
	mainSprite->setRotation(90);
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_01.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_02.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_03.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_04.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_05.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_06.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_07.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_08.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_09.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_10.png", Rect(0, 0, 85, 96)));

	// Create animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animateIdle = Animate::create(animation);

	// Run Animation and repeat it forever
	mainSprite->runAction(RepeatForever::create(animateIdle));
}

// Jump to TargetX
void GameEnemy::Jump(float LTargetX, float RTargetX, float height)
{
	mainSprite->stopAllActions();

	// Reset rotation for jump sprites
	mainSprite->setRotation(0);
	float TargetY = mainSprite->getPosition().y;
	Vec2 vec = Vec2(0, TargetY);
	float duration = Vec2(RTargetX, TargetY).length() * fSpeed;

	// Currently on Right side, Target Left side
	if (eDir == eRight2)
	{
		vec.x = LTargetX;
		mainSprite->setFlippedX(true);
		eDir = eLeft2;
	}
	// Currently on Left side, Target Right side
	else if (eDir == eLeft2)
	{
		vec.x = RTargetX;
		mainSprite->setFlippedX(false);
		eDir = eRight2;
	}
	auto jumpEvent = JumpTo::create(duration, vec, height, 1);

	// Animation frames
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(5);

	animFrames.pushBack(SpriteFrame::create("samurai_run_right_01.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_02.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_03.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_04.png", Rect(0, 0, 85, 96)));
	animFrames.pushBack(SpriteFrame::create("samurai_run_right_05.png", Rect(0, 0, 85, 96)));

	// Create animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animateJump = Animate::create(animation);

	// Run Animation
	mainSprite->runAction(animateJump);

	// Function callback to stop Character jumping animation
	auto callbackRun = CallFunc::create([]()
	{
		auto scene = Director::getInstance()->getRunningScene();
		HelloWorld* HWScene = dynamic_cast<HelloWorld*>(scene);
		if (HWScene != NULL)
		{
			HWScene->getEnemyChar()->Run(); // Call run function to change animation back to run
			HWScene->getEnemyChar()->setStatus(eRun2);
		}
	});

	// Setting sequence of actions to jump
	auto seq = Sequence::create(jumpEvent, callbackRun, nullptr);
	mainSprite->runAction(seq);
}
