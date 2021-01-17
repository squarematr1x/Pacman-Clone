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
		default:
			break;
	}

	validatePos(next_pos, world);
}

void Pacman::validatePos(position next_pos, World& world)
{
	int next_x = static_cast<int>(std::round(next_pos.x));
	int next_y = static_cast<int>(std::round(next_pos.y));

	if (world.isWall(next_y, next_x))
		return;

	updateWorld(world, next_y, next_x);

	int x = static_cast<int>(std::round(m_pos.x));
	int y = static_cast<int>(std::round(m_pos.y));

	world.updatePlayerPos(y, x, next_y, next_x);

	// int dest_x = static_cast<int>(((m_pos.x + next_pos.x) / 2) * 32); // This doesn't work when going through 'pipe' (pacman is rendered at the center)
	// int dest_y = static_cast<int>(((m_pos.y + next_pos.y) / 2) * 32);

	SDL_Rect dest = { next_x*32, next_y*32, 32, 32 };
	m_sprite->update(dest, m_dir);

	m_pos = next_pos;
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
			world.resolveCollision(y, x, RED);
			break;
		case 'P':
			world.resolveCollision(y, x, PINK);
			break;
		case 'B':
			world.resolveCollision(y, x, BLUE);
			break;
		case 'O':
			world.resolveCollision(y, x, ORANGE);
			break;
		default:
			break;
	}
}

void Pacman::reset()
{
	removeLife();
	setPos({ 11.0f, 20.0f });
}
