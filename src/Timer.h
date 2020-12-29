#pragma once

#include <cstdint>

#include <SDL.h>

class Timer
{
public:
	void reset();
	void update();
	void pause();
	void resume();
	void togglePause();

	uint32_t time();

	bool paused() const { return m_paused; }

private:
	uint32_t m_t1{ 0 };
	uint32_t m_t2{ 0 };
	uint32_t m_pt1{ 0 };
	uint32_t m_pt2{ 0 };

	bool m_paused{ false };
};
