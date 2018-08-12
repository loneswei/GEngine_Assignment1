#ifndef __ENEMY_SCENE_H__
#define __ENEMY_SCENE_H__

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
	enum ENEMY_DIRECTION
	{
		ENEMY_LEFT,
		ENEMY_RIGHT,
	};

	// Constructors, Destructor
	Enemy();
	Enemy(ENEMY_TYPE enemyType);
    ~Enemy();

	// Functions
	void EnemyUpdate(float dt);
	void Run();

	// Getters
	ENEMY_TYPE getEnemyType(void) { return enemyType; }
	bool getIsActive(void) { return isActive; }
	Sprite* getEnemySprite(void) { return enemySprite; }
	ENEMY_DIRECTION getEnemyDirection(void) { return enemyDir; }

	// Setters
	void setEnemyType(ENEMY_TYPE _enemyType) { enemyType = _enemyType; }
	void setIsActive(bool _isActive) { isActive = _isActive; }
	void setEnemySprite(Sprite* _enemySprite) { enemySprite = _enemySprite; }
	void setEnemyDirection(ENEMY_DIRECTION _enemyDir) { enemyDir = _enemyDir; }

	//Sprite Sizes
	float SpriteWidth, halfSpriteWidth;

private:
	ENEMY_TYPE enemyType;
	bool isActive;
	Sprite *enemySprite;
	ENEMY_DIRECTION enemyDir;
};

#endif // ____ENEMY_SCENE_H__