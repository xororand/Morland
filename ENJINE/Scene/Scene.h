#pragma once
#include "SFML/Graphics.hpp"
#include <Object/RootObj/RootObj.h>
#include <Network/NetworkManager.h>

using namespace sf;

class SceneManager;
class Scene
{
public:
    enum Type {
        LauncherScene,
        WorldScene
    };

    Scene::Type getType() { return m_type; }
    void setType(Scene::Type t) { m_type = t; }

    void process() {
        this->onProcess();
        this->onDraw();
        this->omUpdateEvents();
    }

    virtual void onProcess() {}
    virtual void onDraw() {}
    virtual void omUpdateEvents() {}

    virtual ~Scene() = default;

    sf::Clock deltaClock;
    Scene::Type m_type = LauncherScene;

    SceneManager* m_scenemg;
};

