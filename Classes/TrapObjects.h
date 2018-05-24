#ifndef __TRAPOBJECT_SCENE_H__
#define ____TRAPOBJECT_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class TrapObject
{
public:
	enum TRAP_TYPE
	{
		TRAP_RANDOM = -1,
		TRAP_SPIKES,
		TRAP_SHURIKEN,

		TRAPS_TOTAL
	};

	TrapObject();
	TrapObject(TRAP_TYPE trapType);
    ~TrapObject();

	void TrapUpdate(float dt);

	// Getters
	TRAP_TYPE gettrapType(void) { return trapType; }
	bool getIsActive(void) { return isActive; }
	Sprite* gettrapSprite(void) { return trapSprite; }

	// Setters
	void settrapType(TRAP_TYPE _trapType) { trapType = _trapType; }
	void setIsActive(bool _isActive) { isActive = _isActive; }
	void settrapSprite(Sprite* _trapSprite) { trapSprite = _trapSprite; }

	TRAP_TYPE trapType;
	bool isActive;
	Sprite *trapSprite;
};

#endif // ____TRAPOBJECT_SCENE_H__