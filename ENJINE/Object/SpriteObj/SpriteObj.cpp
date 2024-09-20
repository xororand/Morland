#include "SpriteObj.h"

void SpriteObj::centered(bool b)
{
	if (b) {
		m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
	}
	else {
		m_sprite.setOrigin(0, 0);
	}
}

void SpriteObj::draw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	sf::Transform combinedTransform = transform * getTransform();

	onDraw(target, combinedTransform);

	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (m_objects[i] == NULL) continue;
		std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object>(m_objects[i]);
		Object::Type objtype = obj.get()->getType();

		// Только объект спрайта может быть отрисован
		if (objtype == Type::SpriteObject) {
			std::shared_ptr<SpriteObj> spriteObj = std::dynamic_pointer_cast<SpriteObj>(obj);

			//combinedTransform = transform * spriteObj.get()->getTransform();
			spriteObj.get()->draw(target, combinedTransform);
		}
	}	
}

void SpriteObj::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_sprite, transform);
}
