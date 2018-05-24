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
	cocos2d::Sprite* getSprite(void) { return mainSprite; }
	EDirection getDirection(void) { return eDir; }
	float getScore(void) { return fScore; }
	EStatus getStatus(void) { return eStat; }
	float getDistanceTravelled(void) { return fDistance; }

	// Setters
	void setStatus(EStatus status) { eStat = status; }
	void setScore(float _score) { fScore = _score; }

private:
	cocos2d::Sprite* mainSprite;
	EDirection eDir;
	EStatus eStat;
	float fSpeed;
	float fScore;
	float fDistance;

};

#endif // __CHAR_SCENE_H__