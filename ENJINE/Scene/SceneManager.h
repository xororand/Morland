#pragma once
#include "Scene.h"

class SceneManager
{
private:
	Scene* m_current_scene;
public:
	SceneManager(Scene* scene_start) {
		m_current_scene = scene_start;
	}
	void setScene(Scene* scene) {
		// TODO: wait for overing scene
		m_current_scene = scene;
	}
	void processScene() {
		m_current_scene->process();
	}
};

