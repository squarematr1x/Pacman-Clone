#include "Pacman.h"

void Pacman::updatePos(Direction dir, World& world)
{
	m_dir = dir;
	position next_pos = m_pos;

	if (dir == Direction::UP)
		next_pos.y -= m_speed;
	else if (dir == Direction::DOWN)
		next_pos.y += m_speed;
	else if (dir == Direction::LEFT)
	{
		if (atLeftmostPos())
			next_pos.x = 22;
		else
			next_pos.x -= m_speed;
	}
	else if (dir == Direction::RIGHT)
	{
		if (atRightmostPos())
			next_pos.x = 0;
		else
			next_pos.x += m_speed;
	}

	validatePos(next_pos, world);
}

void Pacman::validatePos(position next_pos, World& world)
{
	int x = static_cast<int>(std::round(m_pos.x));
	int y = static_cast<int>(std::round(m_pos.y));
	int next_x = static_cast<int>(std::round(next_pos.x));
	int next_y = static_cast<int>(std::round(next_pos.y));

	if (!world.isWall(next_y, next_x, true))
	{
		updateWorld(world, next_y, next_x);

		world.updatePlayerPos(y, x, next_y, next_x);

		SDL_Rect dest = { 32 * next_x, 32 * next_y, 32, 32 };
		m_sprite->update(dest, m_dir);

		m_pos = next_pos;
	}
}

void Pacman::updateWorld(World& world, int y, int x)
{
	if (world.tileAt(y, x) == '*')
		m_score += 10;
	else if (world.tileAt(y, x) == '@')
	{
		world.toggleBigPointEatenFlag();
		m_score += 20;
	}
	else if (world.tileAt(y, x) == 'R')
	{
		if (world.redVulnerable())
		{
			world.toggleRedVulnerableFlag();
			world.toggleRedEatenFlag();
			world.setScorePos(x, y);
			world.toggleRenderScoreFlag();
		}
		else if (!world.redEaten())
			world.togglePlayerEatenFlag();
	}
	else if (world.tileAt(y, x) == 'P')
	{
		if (world.pinkVulnerable())
		{
			world.togglePinkVulnerableFlag();
			world.togglePinkEatenFlag();
			world.setScorePos(x, y);
			world.toggleRenderScoreFlag();
		}
		else if (!world.pinkEaten())
			world.togglePlayerEatenFlag();
	}
	else if (world.tileAt(y, x) == 'B')
	{
		if (world.blueVulnerable())
		{
			world.toggleBlueVulnerableFlag();
			world.toggleBlueEatenFlag();
			world.setScorePos(x, y);
			world.toggleRenderScoreFlag();
		}
		else if (!world.blueEaten())
			world.togglePlayerEatenFlag();
	}
	else if (world.tileAt(y, x) == 'O')
	{
		if (world.orangeVulnerable())
		{
			world.toggleOrangeVulnerableFlag();
			world.toggleOrangeEatenFlag();
			world.setScorePos(x, y);
			world.toggleRenderScoreFlag();
		}
		else if (!world.orangeEaten())
			world.togglePlayerEatenFlag();
	}
}

void Pacman::reset()
{
	removeLife();
	setPos({ 11.0f, 20.0f });
}
