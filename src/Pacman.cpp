#include "Pacman.h"

void Pacman::updatePos(Direction dir, World& world)
{
	position next_pos = m_pos;

	updateDir(dir, world);
	move(next_pos);
	confirmPos(next_pos, world);
}

void Pacman::move(position& next_pos)
{
	switch (m_dir)
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
			next_pos.x = 22.0f;
		else
			next_pos.x -= m_speed;
		break;
	}
	case Direction::RIGHT:
	{
		if (atRightmostPos())
			next_pos.x = 0.0f;
		else
			next_pos.x += m_speed;
		break;
	}
	default:
		break;
	}
}

void Pacman::updateDir(Direction dir, World& world)
{
	switch (dir)
	{
	case Direction::UP:
		if (!world.isWall(static_cast<int>(std::round(m_pos.y)) - 1, static_cast<int>(std::round(m_pos.x))))
			m_dir = dir;
		break;
	case Direction::DOWN:
		if (!world.isWall(static_cast<int>(std::round(m_pos.y)) + 1, static_cast<int>(std::round(m_pos.x))))
			m_dir = dir;
		break;
	case Direction::LEFT:
		if (!world.isWall(static_cast<int>(std::round(m_pos.y)), static_cast<int>(std::round(m_pos.x)) - 1))
			m_dir = dir;
		break;
	case Direction::RIGHT:
		if (!world.isWall(static_cast<int>(std::round(m_pos.y)), static_cast<int>(std::round(m_pos.x)) + 1))
			m_dir = dir;
		break;
	default:
		break;
	}
}

void Pacman::confirmPos(position next_pos, World& world)
{
	int next_x = static_cast<int>(std::round(next_pos.x));
	int next_y = static_cast<int>(std::round(next_pos.y));

	if (world.isWall(next_y, next_x))
		return;

	checkCollision(world, next_y, next_x);

	int x = static_cast<int>(std::round(m_pos.x));
	int y = static_cast<int>(std::round(m_pos.y));

	world.updatePlayerPos(y, x, next_y, next_x);

	int dest_x = static_cast<int>((m_pos.x + 0.5f*(next_pos.x - m_pos.x))*32); // This doesn't work when going through 'pipe' (pacman is rendered at the center)
	int dest_y = static_cast<int>((m_pos.y + 0.5f*(next_pos.y - m_pos.y))*32);

	SDL_Rect dest = { dest_x, dest_y, tile_len, tile_len };
	m_sprite->update(dest, m_dir);

	m_pos = next_pos;
}

void Pacman::checkCollision(World& world, int y, int x)
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

void Pacman::setDead()
{ 
	m_dead = true; 

	m_sprite->setSrcRectX(0);
	m_sprite->incSrcRectY(tile_len);
}

void Pacman::strafeSprite(int pixels)
{
	m_sprite->incSrcRectX(pixels);
}

void Pacman::reset()
{
	removeLife();
	setPos({ 11.0f, 20.0f });

	m_dead = false;
}
