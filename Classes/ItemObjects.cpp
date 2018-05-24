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
	//if (itemType == ITEM_RANDOM)
	//{
	//	this->itemType = static_cast<ITEM_TYPE>(RandomHelper::random_int(0, (int)ITEM_TOTAL - 1));
	//}
}

ItemObject::~ItemObject()
{
}

void ItemObject::ItemUpdate(float dt)
{
	if (itemType != ITEM_COIN)
	{
		//Update position of item
		itemSprite->setPositionY(itemSprite->getPositionY() - ITEM_SPEED * dt);
	}
}
