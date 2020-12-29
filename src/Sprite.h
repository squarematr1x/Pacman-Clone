#pragma once

#include "TextureManager.h"

#include <iostream>

enum class Mode
{
	REST,
	START,
	CHASE,
	SCATTER,
	FLEE,
	EATEN
};

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Sprite
{
public:
	Sprite(const char* sprite_file, SDL_Renderer* renderer, SDL_Rect src = { 0, 192, 32, 32 }, SDL_Rect dest = { 11 * 32, 20 * 32, 32, 32 })
		: m_texture(loadTexture(sprite_file, renderer)),
		  m_renderer(renderer),
		  m_initial_src(src),
		  m_src(src),
		  m_dest(dest)
	{
	}

	~Sprite()
	{
		SDL_DestroyTexture(m_texture);
	}

	void update(SDL_Rect dest, Direction dir, Mode mode = Mode::REST, bool flee_ending = false);
	void draw();

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	SDL_Rect m_initial_src;
	SDL_Rect m_src;
	SDL_Rect m_dest;
};
