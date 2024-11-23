#pragma once
#include <mutex>
#include "SFML/Graphics.hpp"
#include "Object/mGame/G_RootObj/G_RootObj.h"
#include <mGame/Managers/Network/NetworkManager.h>

using namespace sf;

class SceneManager;

class Scene
{
public:
    enum Type {
        LauncherScene,
        WorldScene
    };
private:
    std::wstring m_name = L"";

    Game* m_game = nullptr;
    int m_current_page = 0;
public:
    void setGame(Game* game)        { m_game = game; }
    Game* getGame()                 { return m_game; }

    Scene::Type getType()           { return m_type; }
    void setType(Scene::Type t)     { m_type = t; }

    std::wstring getName()          { return m_name; }
    void setName(std::wstring name);

    void setPage(int page)         { m_current_page = page; }
    int getPage()                  { return m_current_page; }

    void process() {
        this->onUpdateEvents();
        this->onProcess();
        this->onDraw();
    }

    // Общая логика сцены
    virtual void onProcess() {}
    virtual void onDraw() {}
    virtual void onUpdateEvents() {}

    virtual ~Scene() = default;

    sf::Clock deltaClock;
    Scene::Type m_type = Scene::Type::LauncherScene;
};

