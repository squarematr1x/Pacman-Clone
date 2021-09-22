#include "GameObject.h"

// This should/could be friend
int GameObject::lerp(float current_pos, float next_pos)
{
	float k = 0.5f;

	return static_cast<int>((current_pos + k * (next_pos - current_pos)) * tile_len);
}

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
	return (m_pos.x <= 0 && m_pos.y > 12 && m_pos.y < 14);
}

bool GameObject::atRightmostPos()
{
	return (m_pos.x >= 22 && m_pos.y > 12 && m_pos.y < 14);
}

bool GameObject::movingVertically()
{
	return (m_dir == Direction::UP || m_dir == Direction::DOWN);
}

bool GameObject::movingHorizontally()
{
	return (m_dir == Direction::LEFT || m_dir == Direction::RIGHT);
}


void Ghost::changeMode(Mode mode)
{ 
	switch (mode)
	{
		case Mode::FLEE:
			m_flee_ending = false;
			break;
		case Mode::EATEN:
			setSpeed(0.275f);
			break;
		case Mode::SCATTER:
		case Mode::CHASE:
			m_reverse_dir = true;
			break;
	}

	setMode(mode); 
}

void Ghost::start(int x, int y, World& world)
{
	if (x == 11.0f && y == 11.0f)
	{
		if (m_mode == Mode::EATEN)
		{
			reset(world);
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

	switch (last_dir)
	{
		case Direction::UP:
			possible_pos.insert({ Direction::DOWN,  { m_pos.x, m_pos.y + 1.0f } });
			break;
		case Direction::DOWN:
			possible_pos.insert({ Direction::UP,    { m_pos.x, m_pos.y - 1.0f } });
			break;
		case Direction::LEFT:
			possible_pos.insert({ Direction::RIGHT, { m_pos.x + 1.0f, m_pos.y } });
			break;
		case Direction::RIGHT:
			possible_pos.insert({ Direction::LEFT,  { m_pos.x - 1.0f, m_pos.y } });
			break;
		default:
			break;
	}

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
	Direction last_dir = m_dir;
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

	int dest_x = x * tile_len;
	int dest_y = y * tile_len;

	if (m_dir == last_dir)
	{
		if (movingVertically())
			dest_y = lerp(m_pos.y, m_pos.y + m_speed);
		else if (movingHorizontally())
			dest_x = lerp(m_pos.x, m_pos.x + m_speed);
	}
	else
	{
		m_pos.y = std::round(m_pos.y);
		m_pos.x = std::round(m_pos.x);
	}

	SDL_Rect sprite_dest = { dest_x, dest_y, tile_len, tile_len };
	m_sprite->update(sprite_dest, m_dir, m_mode, m_flee_ending);
}

void Ghost::updatePos(int x, int y, Direction dir, World& world)
{
	updateLastPos(x, y, world);

	switch (dir)
	{
		case Direction::UP:
			m_pos.y -= m_speed;
			break;
		case Direction::DOWN:
			m_pos.y += m_speed;
			break;
		case Direction::LEFT:
		{
			if (atLeftmostPos())
				m_pos.x = 22.0f;
			else
				m_pos.x -= m_speed;
			break;
		}
		case Direction::RIGHT:
		{
			if (atRightmostPos())
				m_pos.x = 0.0f;
			else
				m_pos.x += m_speed;
			break;
		}
		default:
			break;
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

	char next_tile = world.tileAt(new_y, new_x);

	if (next_tile == 'C')
	{
		if (canEatPlayer())
			world.togglePlayerEatenFlag();
		else
		{
			if (m_mode != Mode::EATEN)
			{
				setToEaten(world);
				world.toggleRenderScoreFlag();
				world.togglePlayerScoreFlag();
			}
		}
	}

	setMapTile(world, next_tile, new_x, new_y);
}

bool Ghost::canEatPlayer()
{
	return (m_mode == Mode::SCATTER || m_mode == Mode::CHASE);
}

position Red::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	(void)red_pos;
	(void)pacman_dir;

	return pacman_pos;
}

void Red::reset(World& world)
{
	if (world.redVulnerable())
		world.toggleRedVulnerableFlag();

	if (world.redEaten())
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
	world.setTileAt(y, x, 'R');
}

position Pink::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	(void)red_pos;
	position offset{ 0.0f, 0.0f };

	switch (pacman_dir)
	{
		case Direction::UP:
			offset = { 0.0f, -4.0f };
			break;
		case Direction::DOWN:
			offset = { 0.0f, 4.0f };
			break;
		case Direction::LEFT:
			offset = { -4.0f, 0.0f };
			break;
		case Direction::RIGHT:
			offset = { 4.0f, 0.0f };
			break;
		default:
			break;
	}

	return pacman_pos + offset;
}

void Pink::reset(World& world)
{
	if (world.pinkVulnerable())
		world.togglePinkVulnerableFlag();

	if (world.pinkEaten())
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
	world.setTileAt(y, x, 'P');
}

position Blue::getChasePos(position pacman_pos, position red_pos, Direction pacman_dir)
{
	position offset{ 0.0f, 0.0f };

	switch (pacman_dir)
	{
		case Direction::UP:
			offset = { 0.0f, -2.0f };
			break;
		case Direction::DOWN:
			offset = { 0.0f, 2.0f };
			break;
		case Direction::LEFT:
			offset = { -2.0f, 0.0f };
			break;
		case Direction::RIGHT:
			offset = { 2.0f, 0.0f };
			break;
		default:
			break;
	}

	offset = pacman_pos + offset;

	return (2 * offset) - red_pos;
}

void Blue::reset(World& world)
{
	if (world.blueVulnerable())
		world.toggleBlueVulnerableFlag();

	if (world.blueEaten())
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
	world.setTileAt(y, x, 'B');
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
	if (world.orangeVulnerable())
		world.toggleOrangeVulnerableFlag();

	if (world.orangeEaten())
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
	world.setTileAt(y, x, 'O');
}
