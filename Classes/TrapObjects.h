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
	enum TRAP_DIRECTION
	{
		TRAP_LEFT,
		TRAP_RIGHT,
	};

	TrapObject();
	TrapObject(TRAP_TYPE trapType);
    ~TrapObject();

	void TrapUpdate(float dt);

	// Getters
	TRAP_TYPE getTrapType(void) { return trapType; }
	TRAP_DIRECTION getTrapDirection(void) { return trapDir; }
	bool getIsActive(void) { return isActive; }
	Sprite* getTrapSprite(void) { return trapSprite; }

	// Setters
	void setTrapType(TRAP_TYPE _trapType) { trapType = _trapType; }
	void setTrapDirection(TRAP_DIRECTION _trapDir) { trapDir = _trapDir; }
	void setIsActive(bool _isActive) { isActive = _isActive; }
	void setTrapSprite(Sprite* _trapSprite) { trapSprite = _trapSprite; }

private:
	TRAP_TYPE trapType;
	TRAP_DIRECTION trapDir;
	bool isActive;
	Sprite *trapSprite;
};

#endif // ____TRAPOBJECT_SCENE_H__