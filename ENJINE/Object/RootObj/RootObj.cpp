#include "RootObj.h"
#include "../SpriteObj/SpriteObj.h"

RootObj::RootObj() { }

void RootObj::draw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	sf::Transform combinedTransform = transform * getTransform();

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