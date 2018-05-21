#ifndef __ENEMY_SCENE_H__
#define ____ENEMY_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Enemy
{
public:
	enum ENEMY_TYPE
	{
		ENEMY_RANDOM = -1,
		ENEMY_SAMURAI,

		ENEMY_TOTAL
	};

	Enemy();
	Enemy(ENEMY_TYPE trapType, Vec2 anchor);
    ~Enemy();

	void EnemyUpdate(float dt);

	ENEMY_TYPE enemyType;
	bool isActive;
	Sprite *enemySprite;
};

#endif // ____ENEMY_SCENE_H__