#pragma once
#include "../Object.h"

class RootObj : public Object
{
public:
	RootObj();
	virtual void draw(sf::RenderTarget& target, const sf::Transform& transform = sf::Transform()) const;
};

