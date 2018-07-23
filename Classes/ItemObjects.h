#ifndef __ItemObject_SCENE_H__
#define ____ItemObject_SCENE_H__

#include "cocos2d.h"
#include "Character.h"

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
	enum ITEM_DIRECTION
	{
		ITEM_LEFT,
		ITEM_RIGHT,
	};

	ItemObject();
	ItemObject(ITEM_TYPE itemType);
    ~ItemObject();

	void ItemUpdate(float dt);

	// Getters
	ITEM_TYPE getItemType(void) { return itemType; }
	ITEM_DIRECTION getItemDirection(void) { return itemDir; }
	bool getIsActive(void) { return isActive; }
	Sprite* getItemSprite(void) { return itemSprite; }
	float getCoinScore(void) { return coinScore; }

	// Setters
	void setItemType(ITEM_TYPE _itemType) { itemType = _itemType; }
	void setItemDirection(ITEM_DIRECTION _itemDir) { itemDir = _itemDir; }
	void setIsActive(bool _isActive) { isActive = _isActive; }
	void setItemSprite(Sprite* _itemSprite) { itemSprite = _itemSprite; }
	void setCoinScore(float _coinScore) { coinScore = _coinScore; }

	//Sprite Sizes
	float SpriteWidth, halfSpriteWidth;

private:
	ITEM_TYPE itemType;
	ITEM_DIRECTION itemDir;
	bool isActive;
	Sprite *itemSprite;
	float coinScore;
};

#endif // ____ItemObject_SCENE_H__