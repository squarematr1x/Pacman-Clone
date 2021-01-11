#include "Pacman.h"

void Pacman::updatePos(Direction dir, World& world)
{
	m_dir = dir;
	position next_pos = m_pos;

	switch (dir)
	{
		case Direction::UP:
			next_pos.y -= m_speed;
			break;
		case Direction::DOWN:
			next_pos.y += m_speed;
			break;
		case Direction::LEFT:
		{
			if (atLeftmostPos())
				next_pos.x = 22;
			else
				next_pos.x -= m_speed;
			break;
		}
		case Direction::RIGHT:
		{
			if (atRightmostPos())
				next_pos.x = 0;
			else
				next_pos.x += m_speed;
			break;
		}
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
	switch (world.tileAt(y, x))
	{
	case '*':
		m_score += 10;
		break;
	case '@':
		world.toggleBigPointEatenFlag();
		m_score += 20;
		break;
	case 'R':
		world.handleCollision(y, x, RED);
		break;
	case 'P':
		world.handleCollision(y, x, PINK);
		break;
	case 'B':
		world.handleCollision(y, x, BLUE);
		break;
	case 'O':
		world.handleCollision(y, x, ORANGE);
		break;
	}
}

void Pacman::reset()
{
	removeLife();
	setPos({ 11.0f, 20.0f });
}
