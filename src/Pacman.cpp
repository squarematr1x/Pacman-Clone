#include "Pacman.h"

int lerp(float current_pos, float next_pos)
{
	float k = 0.5f;

	return static_cast<int>((current_pos + k * (next_pos - current_pos)) * tile_len);
}

void Pacman::updatePos(Direction dir, World& world)
{
	Direction prev_dir = m_dir;
	position next_pos = m_pos;

	updateDir(dir, next_pos, world);

	if (m_dir != prev_dir)
		m_changed_dir = true;
	else
		m_changed_dir = false;

	move(next_pos);
	confirmPos(next_pos, world);
}

void Pacman::updateDir(Direction dir, position& next_pos, World& world)
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

void Pacman::confirmPos(position next_pos, World& world)
{
	int next_x = static_cast<int>(std::round(next_pos.x));
	int next_y = static_cast<int>(std::round(next_pos.y));

	if (world.isWall(next_y, next_x))
	{
		// std::cout << "Against wall\n";
		m_pos.x = std::round(m_pos.x);
		m_pos.y = std::round(m_pos.y);

		return;
	}

	int x = static_cast<int>(std::round(m_pos.x));
	int y = static_cast<int>(std::round(m_pos.y));

	checkCollision(world, next_y, next_x);
	world.updatePlayerPos(y, x, next_y, next_x);
	
	int dest_x = x * tile_len;
	int dest_y = y * tile_len;

	if (atRightmostPos() || atLeftmostPos())
	{
		dest_x = x * tile_len;
		dest_y = y * tile_len;
	}
	else if (!m_changed_dir && !m_against_wall)
	{
		if (m_dir == Direction::UP || m_dir == Direction::DOWN)
			dest_y = lerp(m_pos.y, next_pos.y);
		else if (m_dir == Direction::LEFT || m_dir == Direction::RIGHT)
			dest_x = lerp(m_pos.x, next_pos.x);
	}

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
