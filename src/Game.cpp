#include "Game.h"

Game::Game(const char* title, int x_pos, int y_pos, int width, int height)
{
	init(title, x_pos, y_pos, width, height);
}

Game::~Game()
{
	clean();
}

void Game::init(const char* title, int x_pos, int y_pos, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_window = SDL_CreateWindow(title, x_pos, y_pos, width, height, SDL_WINDOW_SHOWN);

		if (m_window)
		{
			SDL_Surface* window_icon = IMG_Load("Sprites/icon.png");
			SDL_SetWindowIcon(m_window, window_icon);
			SDL_FreeSurface(window_icon);
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, 0);

		if (m_renderer)
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	}
	else
		m_running = false;

	m_world.loadDetails(m_renderer);

	m_pacman = new Pacman(new Sprite("Sprites/sprite_sheet.png", m_renderer));

	int x = 0, y = 0;
	int w = 32, h = 32;

	m_ghosts.push_back(new Red(new Sprite("Sprites/sprite_sheet.png", m_renderer, {x, y, w, h}, { 12*w, 13*w, w, h })));
	m_ghosts.push_back(new Pink(new Sprite("Sprites/sprite_sheet.png", m_renderer, { x, w, w, h }, { 10*w, 13*w, w, h })));
	m_ghosts.push_back(new Blue(new Sprite("Sprites/sprite_sheet.png", m_renderer, { x, 2*w, w, h }, { 9*w, 13*w, w, h })));
	m_ghosts.push_back(new Orange(new Sprite("Sprites/sprite_sheet.png", m_renderer, { x, 3*w, w, h }, { 13*w, 13*w, w, h })));

	TTF_Init();
	SDL_Color color = { 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("Fonts/arial.ttf", 25);

	if (font == nullptr)
	{
		fprintf(stderr, "error: font not found\n");
		exit(EXIT_FAILURE);
	}

	SDL_Surface* surface = TTF_RenderText_Solid(font, "Pause", color);
	TTF_CloseFont(font);

	m_text = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);
	SDL_QueryTexture(m_text, NULL, NULL, 0, 0);

	int text_w = 64, text_h = 32;
	m_text_rect = { width/2 - text_h, 0, text_w, text_h };

	m_timer.reset();
}

void Game::handleInput()
{
	m_next_dir = m_pacman->getDir();

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT: m_running = false; break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE: 
				{
					togglePause();
					m_timer.togglePause();

					if (fleeing())
						m_flee_timer.togglePause();
				} break;
				case SDLK_LEFT:   m_next_dir = Direction::LEFT;  break;
				case SDLK_RIGHT:  m_next_dir = Direction::RIGHT; break;
				case SDLK_UP:     m_next_dir = Direction::UP;    break;
				case SDLK_DOWN:   m_next_dir = Direction::DOWN;  break;
				case SDLK_SPACE:  m_world.printMap();            break;
			}
		} break;
	}
}

void Game::update()
{
	if (pause())
		return;

	m_pacman->updatePos(m_next_dir, m_world);

	// Check if ghost are eaten
	if (m_world.redEaten())
	{
		m_pacman->addToScore(200);
		m_ghosts[RED]->changeMode(Mode::EATEN);
	}

	if (m_world.pinkEaten())
	{
		m_pacman->addToScore(200);
		m_ghosts[PINK]->changeMode(Mode::EATEN);
	}

	if (m_world.blueEaten())
	{
		m_pacman->addToScore(200);
		m_ghosts[BLUE]->changeMode(Mode::EATEN);
	}

	if (m_world.orangeEaten())
	{
		m_pacman->addToScore(200);
		m_ghosts[ORANGE]->changeMode(Mode::EATEN);
	}

	for (auto ghost : m_ghosts)
		ghost->update(m_pacman->getPos(), m_ghosts[RED]->getPos(), m_pacman->getDir(), m_world);

	if (m_world.playerEaten())
	{
		m_pacman->reset();
		m_world.resetPlayer();

		if (m_pacman->getLives() == 0)
			m_running = false;
	}

	if (m_world.bigPointEaten())
	{
		m_flee = true;

		m_timer.pause();
		m_flee_timer.reset();

		for (auto ghost : m_ghosts)
		{
			if (ghost->getMode() != Mode::REST)
				ghost->changeMode(Mode::FLEE);
		}

		m_world.toggleBigPointEatenFlag();
		m_world.setAllGhostsVulnerable();
	}

	if (releaseRed())
	{
		m_ghosts[RED]->setMode(Mode::START);
		m_red_free = true;
	}

	if (releaseBlue())
	{
		m_ghosts[BLUE]->setMode(Mode::START);
		m_blue_free = true;
	}

	if (m_world.getPoints() == 0)
		m_running = false; // Victory (show text)
	else if (releaseOrange())
	{
		m_orange_free = true;
		m_ghosts[ORANGE]->setMode(Mode::START);
	}

	if (!m_timer.paused())
		m_timer.update();

	if (fleeing())
		m_flee_timer.update();

	updateGhostMode();
}

void Game::updateGhostMode()
{
	if (m_flee_timer.time() >= m_time_limit)
	{
		m_flee = false;
		m_flee_ending = false;
		m_flee_timer.reset();
		m_timer.resume();

		for (auto ghost : m_ghosts)
		{
			if (ghost->getMode() == Mode::FLEE)
			{
				ghost->toggleFleeEndingFlag();
				ghost->setMode(Mode::SCATTER);
			}
		}
	}
	else if (!m_flee_ending && m_flee_timer.time() >= 5000)
	{
		m_flee_ending = true;

		for (auto ghost : m_ghosts)
		{
			if (ghost->getMode() == Mode::FLEE)
				ghost->toggleFleeEndingFlag();
		}
	}

	if (m_timer.time() >= m_time_limit)
	{
		m_timer.reset();

		for (auto ghost : m_ghosts)
		{
			if (ghost->getMode() == Mode::CHASE)
				ghost->changeMode(Mode::SCATTER);
			else if (ghost->getMode() == Mode::SCATTER)
				ghost->changeMode(Mode::CHASE);
			else if (ghost->getMode() == Mode::FLEE)
				ghost->changeMode(Mode::SCATTER);
		}
	}

	// Mode times: STATE MACHINE?
	// Scatter for 7 seconds, then Chase for 20 seconds.
	// Scatter for 7 seconds, then Chase for 20 seconds.
	// Scatter for 5 seconds, then Chase for 20 seconds.
	// Scatter for 5 seconds, then switch to Chase mode permanently.
}

void Game::render()
{
	SDL_RenderClear(m_renderer);

	m_world.render(m_renderer);
	m_pacman->render();

	for (auto ghost : m_ghosts)
		ghost->render();

	if (pause())
		SDL_RenderCopy(m_renderer, m_text, NULL, &m_text_rect);

	SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	for (auto ghost : m_ghosts)
		delete ghost;

	delete m_pacman;

	SDL_DestroyTexture(m_text);
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	TTF_Quit();
	SDL_Quit();
}

bool Game::releaseRed()
{
	if (!m_red_free && m_ghosts[PINK]->escaped())
		return true;

	return false;
}

bool Game::releaseBlue()
{
	if (!m_blue_free && m_pacman->getScore() > 300)
		return true;

	return false;
}

bool Game::releaseOrange()
{
	if (!m_orange_free && m_world.getPoints() == 130)
		return true;

	return false;
}
