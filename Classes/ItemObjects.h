#ifndef __ItemObject_SCENE_H__
#define ____ItemObject_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ItemObject
{
public:
	enum TRAP_TYPE
	{
		TRAP_RANDOM = -1,
		TRAP_SPIKES,

		TRAPS_TOTAL
	};

	ItemObject();
	ItemObject(TRAP_TYPE trapType);
    ~ItemObject();

	void TrapUpdate(float dt);

	TRAP_TYPE trapType;
	bool isActive;
	Sprite *trapSprite;
};

#endif // ____ItemObject_SCENE_H__