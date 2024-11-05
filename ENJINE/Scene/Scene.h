#pragma once
#include <mutex>
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

    Scene::Type getType()       { return m_type; }
    void setType(Scene::Type t) { m_type = t; }

    void process() {
        this->onProcess();
        this->onDraw();
        this->onUpdateEvents();
    }

    // Общая логика сцены
    virtual void onProcess() {}
    virtual void onDraw() {}
    virtual void onUpdateEvents() {}

    SceneManager* getSceneManager()         { return m_scenemg; }
    void setSceneManager(SceneManager* smg) { m_scenemg = smg; }

    virtual ~Scene() = default;

    sf::Clock deltaClock;
    Scene::Type m_type = LauncherScene;
private:
    SceneManager* m_scenemg;
};

