#include "Sprite.h"

void Sprite::draw()
{
	SDL_RenderCopy(m_renderer, m_texture, &m_src, &m_dest);
}

void Sprite::update(SDL_Rect dest, Direction dir, Mode mode, bool flee_ending)
{ 
	bool animate = true;

	if (mode == Mode::FLEE)
	{
		m_src.y = 128;

		if (flee_ending)
			m_src.x = 64;
		else
			m_src.x = 0;
	}
	else
	{
		if (mode == Mode::EATEN)
		{
			m_src.y = 160;
			animate = false;
		}
		else
			m_src.y = m_initial_src.y;

		switch (dir)
		{
			case Direction::UP:    m_src.x = m_src.w * 4; break;
			case Direction::DOWN:  m_src.x = m_src.w * 6; break;
			case Direction::LEFT:  m_src.x = m_src.w * 2; break;
			case Direction::RIGHT: m_src.x = 0;           break;
		}
	}

	if (animate)
		m_src.x += m_src.w * static_cast<int>((SDL_GetTicks() / 200) % 2);

	m_dest = dest; 
}
