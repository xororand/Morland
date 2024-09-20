#pragma once
#include "SFML/Graphics.hpp"
#include <Object/RootObj/RootObj.h>
#include <Network/NetworkManager.h>

using namespace sf;

class Scene
{
public:
    enum Type {
        MainMenuScene,
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
    RenderWindow* m_window;
    NetworkManager* m_netmg;
    RootObj m_root;
    Scene::Type m_type = MainMenuScene;
};

