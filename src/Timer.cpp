#include "Timer.h"

void Timer::reset()
{
	m_t1 = SDL_GetTicks();
	m_t2 = m_t1;
}

void Timer::update()
{
	m_t2 = SDL_GetTicks();
}

void Timer::pause()
{
	m_pt1 = SDL_GetTicks();
	m_paused = true;
}

void Timer::resume()
{
	m_pt2 = SDL_GetTicks();

	uint32_t pause_t = m_pt2 - m_pt1;

	m_t1 += pause_t;
	m_t2 += pause_t;

	m_paused = false;
}

void Timer::togglePause()
{ 
	m_paused = !m_paused; 

	if (paused())
		pause();
	else
		resume();
}

uint32_t Timer::time()
{
	return m_t2 - m_t1;
}
