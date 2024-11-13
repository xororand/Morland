#include "G_RootObj.h"
#include "../G_SpriteObj/G_SpriteObj.h"

G_RootObj::G_RootObj() { }

void G_RootObj::draw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	sf::Transform combinedTransform = transform * getTransform();

	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (m_objects[i] == NULL) continue;
		std::shared_ptr<G_Object> obj = std::dynamic_pointer_cast<G_Object>(m_objects[i]);
		G_Object::Type objtype = obj.get()->getType();

		// Только объект спрайта может быть отрисован
		if (objtype == Type::SpriteObject) {
			std::shared_ptr<G_SpriteObj> spriteObj = std::dynamic_pointer_cast<G_SpriteObj>(obj);

			//combinedTransform = transform * spriteObj.get()->getTransform();
			spriteObj.get()->draw(target, combinedTransform);
		}
	}
}