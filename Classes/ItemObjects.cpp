#include "ItemObjects.h"

static const float ITEM_SPEED = 200;

ItemObject::ItemObject() :
	isActive(true),
	CoinBeingCollected(false)
{
}

ItemObject::ItemObject(ITEM_TYPE itemType) :
	isActive(true),
	itemType(itemType),
	CoinBeingCollected(false)
{
}

ItemObject::~ItemObject()
{
}

void ItemObject::ItemUpdate(float dt)
{
	 
}
