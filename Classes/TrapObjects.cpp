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
	if (trapType == TRAP_RANDOM)
	{
		this->trapType = static_cast<TRAP_TYPE>(RandomHelper::random_int(0, (int)TRAPS_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->trapType == TRAP_SPIKES)
	{
		trapSprite = Sprite::create("spiketrap.png");
		// Hide the remaining 9 traps that are not used
		trapSprite->setPosition(Vec2(-trapSprite->getContentSize().width, -trapSprite->getContentSize().height));
		trapSprite->setRotation(90);
	}

	trapSprite->retain();
}

TrapObject::~TrapObject()
{
	trapSprite->release();
}

void TrapObject::TrapUpdate(float dt)
{
	//Update position of trap
	trapSprite->setPositionY(trapSprite->getPositionY() - TRAP_SPEED * dt);
}
