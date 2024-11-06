#include "Scene.h"
#include "SceneManager.h"
#include "mGame/Game.h"

void Scene::setName(std::wstring name)
{
    m_name = name;
    m_game->getRenderWindow()->setTitle(name);
}
