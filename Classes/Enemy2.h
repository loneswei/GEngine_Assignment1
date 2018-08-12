#pragma once
#ifndef __ENEMY2_SCENE_H__
#define __ENEMY2_SCENE_H__

#include "cocos2d.h"
#include "SaveData.h"

using namespace cocos2d;

class GameEnemy
{
public:
	enum EDirection2 {
		eLeft2 = -1,
		eRight2 = 1,
	};

	enum EStatus2 {
		eRun2,
		eJump2,
	};

	GameEnemy();
	~GameEnemy();
	void init(const char * filename, Vec2 anchor, float x, float y, const char * name);
	void Update(float delta);

	// Functions
	void Run();
	void Jump(float LTargetX, float RTargetX, float height);

	// Getters
	Sprite* getSprite(void) { return mainSprite; }
	Sprite* getShieldSprite(void) { return shieldSprite; }

	EDirection2 getDirection(void) { return eDir; }
	EStatus2 getStatus(void) { return eStat; }

	float getScore(void) { return fScore; }
	float getDistanceTravelled(void) { return fDistance; }
	float getMagnetTimer(void) { return magnetTimer; }
	float getMagnetDuration(void) { return magnetDuration; }
	float getShieldTimer(void) { return shieldTimer; }
	float getShieldDuration(void) { return shieldDuration; }
	float getInvulTimer(void) { return invulTimer; }
	float getInvulDuration(void) { return invulDuration; }

	bool getMagnetActive(void) { return magnetActive; }
	bool getShieldActive(void) { return shieldActive; }
	bool getInvulActive(void) { return invulActive; }
	bool getAliveorNot(void) { return dead; }
	int getLifeCount(void) { return lifeCount; }

	// Setters
	void setStatus(EStatus2 status) { eStat = status; }

	void setScore(float _score) { fScore = _score; }

	void setMagnetActive(bool _magnetActive) { magnetActive = _magnetActive; }
	void setMagnetTimer(float _magnetTimer) { magnetTimer = _magnetTimer; }
	void setShieldActive(bool _shieldActive) { shieldActive = _shieldActive; }
	void setShieldTimer(float _shieldTimer) { shieldTimer = _shieldTimer; }
	void setInvulActive(bool _invulActive) { invulActive = _invulActive; }

	void setLifeCount(int _lifeCount) { lifeCount = _lifeCount; }
	void setAliveorNot(bool _dead) { dead = _dead; }

private:
	Sprite * mainSprite;
	Sprite* shieldSprite;

	EDirection2 eDir;
	EStatus2 eStat;

	float fSpeed;
	float fScore;
	float fDistance;
	float magnetTimer;
	float magnetDuration;
	float shieldTimer;
	float shieldDuration;
	float invulTimer;
	float invulDuration;

	bool magnetActive;
	bool shieldActive;
	bool invulActive;
	bool dead;

	int lifeCount;
	std::string EquippedSkinName;
};

#endif // __CHAR_SCENE_H__