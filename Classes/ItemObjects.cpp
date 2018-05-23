#include "ItemObjects.h"

static const float ITEM_SPEED = 200;

ItemObject::ItemObject() :
	isActive(true)
{
}

ItemObject::ItemObject(ITEM_TYPE itemType) :
	isActive(true),
	itemType(itemType)

{
	if (itemType == ITEM_RANDOM)
	{
		this->itemType = static_cast<ITEM_TYPE>(RandomHelper::random_int(0, (int)ITEM_TOTAL - 1));
	}

	//Assign the sprite texture
	if (this->itemType == ITEM_COIN)
	{
		itemSprite = Sprite::create("coin.png");
		itemSprite->setRotation(90);
	}
	else if (this->itemType == ITEM_SHIELD)
	{
		itemSprite = Sprite::create("shield.png");
	}
	else if (this->itemType == ITEM_MAGNET)
	{
		itemSprite = Sprite::create("magnet.png");
		itemSprite->setRotation(90);
	}

	itemSprite->retain();
}

ItemObject::~ItemObject()
{
	itemSprite->release();
}

void ItemObject::ItemUpdate(float dt)
{
	//Update position of item
	itemSprite->setPositionY(itemSprite->getPositionY() - ITEM_SPEED * dt);
}
