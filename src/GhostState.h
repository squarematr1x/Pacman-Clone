#pragma once

#include "Timer.h"
#include "GameObject.h";

// Scatter time: 7s, 7s, 5s
// Chase time: 20s, 20s, perma
// Flee time: 7s

class GhostState
{
public:
	void update();
	void changeGhostMode();

private:
	unsigned int scatter_time{ 7000 };
	unsigned int chase_time{ 20000 };
	unsigned int flee_time{ 7000 };

	int scatters_left{ 3 }; // When 0, only chase

	Timer timer;
	Timer flee_timer;
};
