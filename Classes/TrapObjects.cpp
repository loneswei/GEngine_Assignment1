#include "TrapObjects.h"

static const float TRAP_SPEED = 250;

TrapObject::TrapObject()
{
	isActive = true;
}
#include <assert.h>
TrapObject::TrapObject(TRAP_TYPE trapType)
{
	isActive = true;
	scale = Vec2(1, 1);
	this->trapType = trapType;

	if (trapType == TRAP_RANDOM)
	{
		this->trapType = static_cast<TRAP_TYPE>(RandomHelper::random_int(0, (int)TRAPS_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->trapType == TRAP_SPIKES)
	{
		trapSprite = Sprite::create("spiketrap.png");
	}
	else if (this->trapType == TRAP_HOLES)
	{

	}

	//Set the position of sprite
	trapSprite->setPosition(50, Director::getInstance()->getVisibleSize().height * 2);
}

TrapObject::~TrapObject()
{
}

void TrapObject::TrapUpdate(float dt)
{
	//Update position of trap
	trapSprite->setPositionY(trapSprite->getPositionY() - (TRAP_SPEED * dt));
}
