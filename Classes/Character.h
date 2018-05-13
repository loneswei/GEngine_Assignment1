#ifndef __CHAR_SCENE_H__
#define __CHAR_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

enum EDirection {
	eLeft = -1,
	eRight = 1,
};

class GameChar
{
private:
	cocos2d::Sprite* mainSprite;
	EDirection eDir;

public:
	GameChar();
	~GameChar();
	void init(const char * filename, Vec2 anchor, float x, float y, const char * name);
	void Update(float delta);
	
	void Run();

	cocos2d::Sprite* getSprite(void) { return mainSprite; }
};

#endif // __CHAR_SCENE_H__