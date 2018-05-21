#include "Enemy.h"

static const float ENEMY_SPEED = 200;

Enemy::Enemy() :
	isActive(true)
{
}

Enemy::Enemy(ENEMY_TYPE enemyType, Vec2 anchor) :
	isActive(true),
	enemyType(enemyType)

{
	if (enemyType == ENEMY_RANDOM)
	{
		this->enemyType = static_cast<ENEMY_TYPE>(RandomHelper::random_int(0, (int)ENEMY_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->enemyType == ENEMY_SAMURAI)
	{
		enemySprite = Sprite::create("samurai_run_left_01.png");
		enemySprite->setAnchorPoint(anchor);
		enemySprite->retain();
		enemySprite->setRotation(-90);
	}
}

Enemy::~Enemy()
{
	enemySprite->release();
}

void Enemy::EnemyUpdate(float dt)
{
	//Update position of trap
	enemySprite->setPositionY(enemySprite->getPositionY() - ENEMY_SPEED * dt);
}
