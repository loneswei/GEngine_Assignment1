#include "ItemObjects.h"

static const float TRAP_SPEED = 200;

ItemObject::ItemObject() :
	isActive(true)
{
}

ItemObject::ItemObject(TRAP_TYPE trapType) :
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
		trapSprite->setRotation(90);
	}

	trapSprite->retain();
}

ItemObject::~ItemObject()
{
	trapSprite->release();
}

void ItemObject::TrapUpdate(float dt)
{
	//Update position of trap
	trapSprite->setPositionY(trapSprite->getPositionY() - TRAP_SPEED * dt);
}
