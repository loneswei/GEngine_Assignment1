#ifndef __TRAPOBJECT_SCENE_H__
#define ____TRAPOBJECT_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

const auto SprikeTrapSprite = Sprite::create("spiketrap.png");

class TrapObject
{
public:
	enum TRAP_TYPE
	{
		TRAP_RANDOM = -1,
		TRAP_SPIKES,
		TRAP_HOLES,

		TRAPS_TOTAL
	};

	TrapObject(TRAP_TYPE trapType);
    ~TrapObject();

	void TrapUpdate(float dt);

	TRAP_TYPE trapType;
	bool isActive;
	Vec2 position, scale;
	Sprite *trapSprite;
};

#endif // ____TRAPOBJECT_SCENE_H__