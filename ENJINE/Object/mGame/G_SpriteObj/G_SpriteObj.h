#pragma once
#include "../../G_Object.h"

class G_SpriteObj : public G_Object {
public:
	G_SpriteObj(sf::Sprite sprite, bool is_centered = true) {
		setType(Type::SpriteObject);
		m_sprite = sprite;
		centered(is_centered);
	}
	void centered(bool b);
	virtual ~G_SpriteObj() = default;
	virtual void draw(sf::RenderTarget& target, const sf::Transform& transform = sf::Transform()) const;
private:
	virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	sf::Sprite m_sprite;
};

