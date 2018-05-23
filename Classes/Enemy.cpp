#include "Enemy.h"

static const float ENEMY_SPEED = 200;

Enemy::Enemy() :
	isActive(false)
{
}

Enemy::Enemy(ENEMY_TYPE enemyType) :
	isActive(false),
	enemyType(enemyType)
{
	if (enemyType == ENEMY_RANDOM)
	{
		this->enemyType = static_cast<ENEMY_TYPE>(RandomHelper::random_int(0, (int)ENEMY_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->enemyType == ENEMY_SAMURAI)
	{
		//enemySprite = Sprite::create("samurai_run_right_01.png");
		enemySprite = Sprite::create("samurai_run_left_01.png");
		// Hide the remaining 9 enemies that are not used
		enemySprite->setPosition(Vec2(-enemySprite->getContentSize().width, -enemySprite->getContentSize().height));
	}
}

Enemy::~Enemy()
{
}

void Enemy::EnemyUpdate(float dt)
{
	//Update position of enemy
	enemySprite->setPositionY(enemySprite->getPositionY() - ENEMY_SPEED * dt);
}

void Enemy::Run()
{
	if (isActive)
	{
		enemySprite->stopAllActions();

		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(10);

		switch (enemyDir)
		{
		case ENEMY_RIGHT:
			// Rotate anti-clockwise by 90(For Enemy's right side)
			enemySprite->setRotation(-90);
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_01.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_02.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_03.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_04.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_05.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_06.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_07.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_08.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_09.png", Rect(0, 0, 85, 96)));
			animFrames.pushBack(SpriteFrame::create("samurai_run_left_10.png", Rect(0, 0, 85, 96)));
			break;
		case ENEMY_LEFT:
			// Rotate clockwise by 90(For Enemy's left side)
			enemySprite->setRotation(90);
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
			break;
		}

		// Create animation out of the frames
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		Animate* animateIdle = Animate::create(animation);

		// Run animation and repeat it forever
		enemySprite->runAction(RepeatForever::create(animateIdle));
	}
}
