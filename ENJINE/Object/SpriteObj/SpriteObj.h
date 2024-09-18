#pragma once
#include "../Object.h"

class SpriteObj : public Object {
public:
	SpriteObj(sf::Sprite sprite) {
		setType(Type::SpriteObject);
		m_sprite = sprite;
	}
	virtual ~SpriteObj() = default;
	virtual void draw(sf::RenderTarget& target, const sf::Transform& transform = sf::Transform()) const;
private:
	virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	sf::Sprite m_sprite;
};

