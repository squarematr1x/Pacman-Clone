#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include <SDL_ttf.h>

#include "Timer.h"
#include "Pacman.h"
#include "World.h"

class Game
{
public:
	Game(const char* title, int x_pos, int y_pos, int width, int height);

	~Game();

	void init(const char* title, int x_pos, int y_pos, int width, int height);
	void handleInput();
	void update();
	void updateGhostMode();
	void render();
	void clean();

	void togglePause() { m_pause = !m_pause; }

	bool running() const { return m_running; }
	bool pause() const { return m_pause; }
	bool fleeing() const { return m_flee; }
	bool releaseRed();
	bool releaseBlue();
	bool releaseOrange();

private:
	bool m_running{ true };
	bool m_pause{ false };
	bool m_red_free{ false };
	bool m_blue_free{ false };
	bool m_orange_free{ false };
	bool m_flee{ false };
	bool m_flee_ending{ false };

	int m_score{ 0 };
	unsigned int m_time_limit{ 7000 };

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	World m_world;
	Direction m_next_dir{ Direction::LEFT };

	Pacman* m_pacman{ nullptr };
	std::vector<Ghost*> m_ghosts;

	SDL_Texture* m_text;
	SDL_Rect m_text_rect;

	Timer m_timer;
	Timer m_flee_timer;
};
