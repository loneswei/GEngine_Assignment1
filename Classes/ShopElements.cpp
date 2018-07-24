#include "ShopElements.h"

ShopElement::ShopElement(const std::string Name, const std::string SpriteFilePath, const unsigned int Price) : 
	Name(Name),
	SpriteFilePath(SpriteFilePath),
	Price(Price)
{
	theSprite = Sprite::create(SpriteFilePath);
	theSprite->setName(Name);

	SpriteWidth = theSprite->getContentSize().width;
	SpriteHeight = theSprite->getContentSize().height;
	halfSpriteWidth = SpriteWidth * 0.5f;
	halfSpriteHeight = SpriteHeight * 0.5f;
}

ShopElement::~ShopElement()
{
}

std::string ShopElement::GetName() const
{
	return Name;
}

std::string ShopElement::GetSpriteFilePath() const
{
	return SpriteFilePath;
}

unsigned int ShopElement::GetPrice() const
{
	return Price;
}

Sprite * ShopElement::GetSprite() const
{
	return theSprite;
}

void ShopElement::SetPosition(Vec2 newPos)
{
	theSprite->setPosition(newPos);
}

void ShopElement::SetPosition(float x, float y)
{
	theSprite->setPosition(x, y);
}

Vec2 ShopElement::GetPosition() const
{
	return theSprite->getPosition();
}
