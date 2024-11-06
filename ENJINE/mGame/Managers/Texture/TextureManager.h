#pragma once

class Game;

class TextureManager
{
private:
	Game* m_game = nullptr;
	// TODO: LOADING ALL TEXTURES TO MEMORY AND COMPILTE TO ATLASES
public:
	TextureManager(Game* game);

	Game* getGame() { return m_game; }

	void loadTexturesFromRootResources();
};

