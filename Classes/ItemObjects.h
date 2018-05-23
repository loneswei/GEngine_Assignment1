#ifndef __ItemObject_SCENE_H__
#define ____ItemObject_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ItemObject
{
public:
	enum ITEM_TYPE
	{
		ITEM_RANDOM = -1,
		ITEM_COIN,
		ITEM_SHIELD,
		ITEM_MAGNET,

		ITEM_TOTAL
	};

	ItemObject();
	ItemObject(ITEM_TYPE itemType);
    ~ItemObject();

	void ItemUpdate(float dt);

	ITEM_TYPE itemType;
	bool isActive;
	Sprite *itemSprite;
};

#endif // ____ItemObject_SCENE_H__