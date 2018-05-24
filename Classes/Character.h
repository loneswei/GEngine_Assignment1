#ifndef __CHAR_SCENE_H__
#define __CHAR_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class GameChar
{
public:
	enum EDirection {
		eLeft = -1,
		eRight = 1,
	};

	enum EStatus {
		eRun,
		eJump,
	};

	GameChar();
	~GameChar();
	void init(const char * filename, Vec2 anchor, float x, float y, const char * name);
	void Update(float delta);
	
	// Functions
	void Run();
	void Jump(float LTargetX, float RTargetX, float height);

	// Getters
	Sprite* getSprite(void) { return mainSprite; }
	Sprite* getShieldSprite(void) { return shieldSprite; }

	EDirection getDirection(void) { return eDir; }
	EStatus getStatus(void) { return eStat; }

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
	
	int getLifeCount(void) { return lifeCount; }

	// Setters
	void setStatus(EStatus status) { eStat = status; }

	void setScore(float _score) { fScore = _score; }

	void setMagnetActive(bool _magnetActive) { magnetActive = _magnetActive; }
	void setShieldActive(bool _shieldActive) { shieldActive = _shieldActive; }
	void setInvulActive(bool _invulActive) { invulActive = _invulActive; }

	void setLifeCount(int _lifeCount) { lifeCount = _lifeCount; }

private:
	Sprite* mainSprite;
	Sprite* shieldSprite;

	EDirection eDir;
	EStatus eStat;

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

	int lifeCount;
};

#endif // __CHAR_SCENE_H__