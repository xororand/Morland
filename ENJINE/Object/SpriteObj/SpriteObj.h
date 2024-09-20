#pragma once
#include "../Object.h"

class SpriteObj : public Object {
public:
	SpriteObj(sf::Sprite sprite, bool is_centered = true) {
		setType(Type::SpriteObject);
		m_sprite = sprite;
		centered(is_centered);
	}
	void centered(bool b);
	virtual ~SpriteObj() = default;
	virtual void draw(sf::RenderTarget& target, const sf::Transform& transform = sf::Transform()) const;
private:
	virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	sf::Sprite m_sprite;
};

