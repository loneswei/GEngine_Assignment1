#include "TrapObjects.h"

static const float TRAP_SPEED = 10;

TrapObject::TrapObject(TRAP_TYPE trapType) :
	isActive(true),
	position(0, 0),
	scale(1, 1),
	trapType(trapType)
{
	if (trapType == TRAP_RANDOM)
	{
		this->trapType = static_cast<TRAP_TYPE>(RandomHelper::random_int(0, (int)TRAPS_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->trapType == TRAP_SPIKES)
	{
		trapSprite = Sprite::createWithTexture(SprikeTrapSprite->getTexture());
	}
	else if (this->trapType == TRAP_HOLES)
	{

	}
}

TrapObject::~TrapObject()
{
}

void TrapObject::TrapUpdate(float dt)
{
	//Update position of trap
}
