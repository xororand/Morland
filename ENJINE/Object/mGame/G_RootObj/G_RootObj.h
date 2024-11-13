#pragma once
#include "../../G_Object.h"

class G_RootObj : public G_Object
{
public:
	G_RootObj();
	virtual void draw(sf::RenderTarget& target, const sf::Transform& transform = sf::Transform()) const;
};

