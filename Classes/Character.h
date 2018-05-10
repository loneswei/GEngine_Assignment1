#ifndef __CHAR_SCENE_H__
#define __CHAR_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

enum EAction {
	eLeft = -1,
	eStop,
	eRight,
};

class GameChar
{
private:
	cocos2d::Sprite* mainSprite;
	EAction eDir;
	float fSpeed;

public:
	GameChar();
	~GameChar();
	void init(const char *, Vec2, float, float, const char *);
	// Relative movement
	void MoveChar(EAction);
	// Absolute movement
	void MoveCharByCoord(float, float);
	void Update(float);
	void Stop();

	cocos2d::Sprite* getSprite(void) { return mainSprite; }
};

#endif // __CHAR_SCENE_H__