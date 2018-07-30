#ifndef __ShopElements_SCENE_H__
#define ____ShopElements_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;


class ShopElement
{
public:

	ShopElement(const std::string Name, const std::string SpriteFilePath, const unsigned int Price = 0);
    ~ShopElement();
	
	//Getters
	std::string GetName() const, GetSpriteFilePath() const;
	unsigned int GetPrice() const;
	Sprite* GetSprite() const;

	void SetPosition(Vec2 newPos), SetPosition(float x = 0, float y = 0);
	Vec2 GetPosition() const;

	float SpriteWidth, halfSpriteWidth;
	float SpriteHeight, halfSpriteHeight;
	bool isBought, isEquipped;

private:
	const std::string Name;
	const std::string SpriteFilePath;
	const unsigned int Price;
	Sprite* theSprite;
};

#endif // ____ShopElements_SCENE_H__