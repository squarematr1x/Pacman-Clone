#include "GameObject.h"

void GameObject::render()
{
	m_sprite->draw();
}

float GameObject::getDistance(position src, position dest)
{
	return static_cast<float>(std::sqrt(std::pow(std::abs(dest.x - src.x), 2) + std::pow(std::abs(dest.y - src.y), 2)));
}

bool GameObject::atLeftmostPos()
{
	return (m_pos.x < 1 && m_pos.y > 12 && m_pos.y < 14);
}

bool GameObject::atRightmostPos()
{
	return (m_pos.x > 22 && m_pos.y > 12 && m_pos.y < 14);
}

void Ghost::changeMode(Mode mode)
{ 
	if (mode == Mode::EATEN)
		setSpeed(0.25f);

	if (m_mode == Mode::SCATTER || m_mode == Mode::CHASE)
		m_reverse_dir = true;

	setMode(mode); 
}

void Ghost::start(int x, int y, World& world)
{
	if (x == 11.0f && y == 11.0f)
	{
		if (m_mode == Mode::EATEN)
		{
			reset(world);
			world.toggleRenderScoreFlag();
			setSpeed(0.125f);

			if (m_flee_ending)
				toggleFleeEndingFlag();
		}

		m_mode = Mode::SCATTER;
		m_escaped = true;
	}
}

std::map<Direction, position> Ghost::getPossiblePos(Direction last_dir)
{
	if (m_reverse_dir)
	{
		m_reverse_dir = false;
		return getOppositePos(last_dir);
	}

	std::map<Direction, position> possible_pos;

	if (last_dir != Direction::UP)
		possible_pos.insert({ Direction::DOWN,  { m_pos.x, m_pos.y + 1.0f } });

	if (last_dir != Direction::DOWN)
		possible_pos.insert({ Direction::UP,    { m_pos.x, m_pos.y - 1.0f } });

	if (last_dir != Direction::LEFT)
		possible_pos.insert({ Direction::RIGHT, { m_pos.x + 1.0f, m_pos.y } });

	if (last_dir != Direction::RIGHT)
		possible_pos.insert({ Direction::LEFT,  { m_pos.x - 1.0f, m_pos.y } });

	return possible_pos;
}

std::map<Direction, position> Ghost::getOppositePos(Direction last_dir)
{
	std::map<Direction, position> possible_pos;

	if (last_dir == Direction::UP)
		possible_pos.insert({ Direction::DOWN,  { m_pos.x, m_pos.y + 1.0f } });
	else if (last_dir == Direction::DOWN)
		possible_pos.insert({ Direction::UP,    { m_pos.x, m_pos.y - 1.0f } });
	else if (last_dir == Direction::LEFT)
		possible_pos.insert({ Direction::RIGHT, { m_pos.x + 1.0f, m_pos.y } });
	else if (last_dir == Direction::RIGHT)
		possible_pos.insert({ Direction::LEFT,  { m_pos.x - 1.0f, m_pos.y } });

	return possible_pos;
}

void Ghost::updateDest(position& dest, position pacman_pos, position red_pos, Direction pacman_dir)
{
	if (m_mode == Mode::CHASE)
		dest = getChasePos(pacman_pos, red_pos, pacman_dir);
	else if (m_mode == Mode::SCATTER || m_mode == Mode::FLEE)
		dest = m_scatter_dest;
}

void Ghost::update(position pacman_pos, position red_pos, Direction pacman_dir, World& world)
{
	if (m_mode == Mode::REST)
		return;

	position dest = { 11.0f, 11.0f };

	int x = static_cast<int>(std::round(m_pos.x));
	int y = static_cast<int>(std::round(m_pos.y));

	m_world_pos.x = x;
	m_world_pos.y = y;

	if (m_mode == Mode::START || m_mode == Mode::EATEN)
		start(x, y, world);

	updateDest(dest, pacman_pos, red_pos, pacman_dir);

	if (m_changed_world_pos)
	{
		float distance = 100.0f;
		std::map<Direction, position> moves = getPossiblePos(m_dir);

		for (auto const& move : moves)
		{
			if (!world.isWall(move.second, m_escaped))
			{
				float new_distance = getDistance(move.second, dest);

				if (new_distance < distance)
				{
					distance = new_distance;
					m_dir = move.first;
				}
			}
		}
	}

	updatePos(x, y, m_dir, world);

	SDL_Rect sprite_dest = { 32 * x, 32 * y, 32, 32 };
	m_sprite->update(sprite_dest, m_dir, m_mode, m_flee_ending);
}

void Ghost::updatePos(int x, int y, Direction dir, World& world)
{
	updateLastPos(x, y, world);

	if (dir == Direction::UP)
		m_pos.y -= m_speed;
	else if (dir == Direction::DOWN)
		m_pos.y += m_speed;
	else if (dir == Direction::LEFT)
	{
		if (atLeftmostPos())
			m_pos.x = 22.0f;
		else
			m_pos.x -= m_speed;
	}
	else if (dir == Direction::RIGHT)
	{
		if (atRightmostPos())
			m_pos.x = 0.0f;
		else
			m_pos.x += m_speed;
	}

	updateMapPos(world);
}

void Ghost::updateMapPos(World& world)
{
	int new_x = static_cast<int>(std::round(m_pos.x));
	int new_y = static_cast<int>(std::round(m_pos.y));

	if (m_world_pos.x != new_x || m_world_pos.y != new_y)
		m_changed_world_pos = true;
	else
		m_changed_world_pos = false;

	char next_tile = world.charAt(new_y, new_x);

	if (next_tile == 'C')
	{
		if (m_mode != Mode::FLEE && m_mode != Mode::EATEN)
			world.togglePlayerEatenFlag();
		else
		{
			setToEaten(world);
			world.setScorePos(new_x, new_y);
			world.toggleRenderScoreFlag();
		}
	}

	setMapTile(world, next_tile, new_x, new_y);
}

position Red::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	(void)red_pos;
	(void)pacman_dir;

	return pacman_pos;
}

void Red::reset(World& world)
{
	world.toggleRedEatenFlag();
}

void Red::setToEaten(World& world)
{
	world.toggleRedVulnerableFlag();
	world.toggleRedEatenFlag();
}

void Red::setMapTile(World& world, char next_tile, int x, int y)
{
	world.setGhostBuffer(next_tile, RED);
	world.setCharAt(y, x, 'R');
}

position Pink::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	(void)red_pos;

	position offset;

	if (pacman_dir == Direction::UP)
		offset = { 0.0f, -4.0f };
	else if (pacman_dir == Direction::DOWN)
		offset = { 0.0f, 4.0f };
	else if (pacman_dir == Direction::LEFT)
		offset = { -4.0f, 0.0f };
	else if (pacman_dir == Direction::RIGHT)
		offset = { 4.0f, 0.0f };

	return pacman_pos + offset;
}

void Pink::reset(World& world)
{
	world.togglePinkEatenFlag();
}

void Pink::setToEaten(World& world)
{
	world.togglePinkVulnerableFlag();
	world.togglePinkEatenFlag();
}

void Pink::setMapTile(World& world, char next_tile, int x, int y)
{
	world.setGhostBuffer(next_tile, PINK);
	world.setCharAt(y, x, 'P');
}

position Blue::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	position offset;

	if (pacman_dir == Direction::UP)
		offset = { 0.0f, -2.0f };
	else if (pacman_dir == Direction::DOWN)
		offset = { 0.0f, 2.0f };
	else if (pacman_dir == Direction::LEFT)
		offset = { -2.0f, 0.0f };
	else if (pacman_dir == Direction::RIGHT)
		offset = { 2.0f, 0.0f };

	offset = pacman_pos + offset;

	return (2 * offset) - red_pos;
}

void Blue::reset(World& world)
{
	world.toggleBlueEatenFlag();
}

void Blue::setToEaten(World& world)
{
	world.toggleBlueVulnerableFlag();
	world.toggleBlueEatenFlag();
}

void Blue::setMapTile(World& world, char next_tile, int x, int y)
{
	world.setGhostBuffer(next_tile, BLUE);
	world.setCharAt(y, x, 'B');
}

position Orange::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	(void)red_pos;
	(void)pacman_dir;

	if (getDistance(m_pos, pacman_pos) < 8.0f)
		return m_scatter_dest;

	return pacman_pos;
}

void Orange::reset(World& world)
{
	world.toggleOrangeEatenFlag();
}

void Orange::setToEaten(World& world)
{
	world.toggleOrangeVulnerableFlag();
	world.toggleOrangeEatenFlag();
}

void Orange::setMapTile(World& world, char next_tile, int x, int y)
{
	world.setGhostBuffer(next_tile, ORANGE);
	world.setCharAt(y, x, 'O');
}
