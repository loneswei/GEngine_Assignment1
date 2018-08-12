#include "TrapObjects.h"

static const float TRAP_SPEED = 200;

TrapObject::TrapObject() :
	isActive(true)
{
}

TrapObject::TrapObject(TRAP_TYPE trapType) :
	isActive(true),
	trapType(trapType)

{
}

TrapObject::~TrapObject()
{
}

void TrapObject::TrapUpdate(float dt)
{
	//Update position of trap
	trapSprite->setPositionY(trapSprite->getPositionY() - TRAP_SPEED * dt);
}
