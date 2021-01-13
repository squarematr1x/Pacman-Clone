#include "World.h"

position operator+(position const& p1, position const& p2)
{
	return { p1.x + p2.x, p1.y + p2.y };
}

position operator*(position const& p, float k)
{
	return { k * p.x, k * p.y };
}

position operator*(float k, position const& p)
{
	return { k * p.x, k * p.y };
}

position operator-(position const& p1, position const& p2)
{
	return { p1.x - p2.x, p1.y - p2.y };
}

World::World()
{
	calculatePoints();
}

World::~World()
{
	SDL_DestroyTexture(m_area_tex);
	SDL_DestroyTexture(m_point_tex);
	SDL_DestroyTexture(m_big_point_tex);
	SDL_DestroyTexture(m_score_tex);
	SDL_DestroyTexture(m_berry_tex);
}

void World::loadDetails(SDL_Renderer* renderer)
{
	m_area_tex = loadTexture("Sprites/area.png", renderer);
	m_point_tex = loadTexture("Sprites/point.png", renderer);
	m_big_point_tex = loadTexture("Sprites/big_point.png", renderer);
	m_score_tex = loadTexture("Sprites/score.png", renderer);
	m_berry_tex = loadTexture("Sprites/berry.png", renderer);
}

void World::render(SDL_Renderer* renderer)
{
	draw(m_area_tex, renderer, area_rect, area_rect);

	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 23; j++)
		{
			char tile = m_map[i][j];

			m_dest.x = j * 32;
			m_dest.y = i * 32;

			if (tile == '*')
				draw(m_point_tex, renderer, m_src, m_dest);
			else if (tile == '@')
				draw(m_big_point_tex, renderer, m_src, m_dest);
			else if (tile == '$')
				draw(m_berry_tex, renderer, m_src, m_dest);
		}
	}
}

void World::renderScore(SDL_Renderer* renderer)
{
	SDL_Rect dest{ m_score_pos.x * 32, m_score_pos.y * 32, 32, 32 };
	draw(m_score_tex, renderer, m_src, dest);
}

void World::resetPlayer()
{
	togglePlayerEatenFlag();
}

void World::setGhostVulnerable(color ghost_color)
{
	switch (ghost_color)
	{
		case RED:
			m_red_vulnerable = true;
			break;
		case PINK:
			m_pink_vulnerable = true;
			break;
		case BLUE:
			m_blue_vulnerable = true;
			break;
		case ORANGE:
			m_orange_vulnerable = true;
			break;
		default:
			break;
	}
}

void World::resolveCollision(int y, int x, color ghost_color)
{
	if (vulnerable(ghost_color))
	{
		toggleVulnerableFlag(ghost_color);
		toggleEatenFlag(ghost_color);
		setScorePos(x, y);
		toggleRenderScoreFlag();
	}
	else if (!eaten(ghost_color))
		togglePlayerEatenFlag();
}

void World::updatePlayerPos(int old_y, int old_x, int new_y, int new_x)
{
	if (m_map[new_y][new_x] == '*')
		m_points--;

	m_map[old_y][old_x] = ' ';
	m_map[new_y][new_x] = 'C';
}

void World::toggleVulnerableFlag(color ghost_color)
{
	switch (ghost_color)
	{
		case RED:    
			toggleRedVulnerableFlag();    
			break;
		case PINK:   
			togglePinkVulnerableFlag();   
			break;
		case BLUE:   
			toggleBlueVulnerableFlag();   
			break;
		case ORANGE: 
			toggleOrangeVulnerableFlag(); 
			break;
		default:
			break;
	}
}

void World::toggleEatenFlag(color ghost_color)
{
	switch (ghost_color)
	{
		case RED:    
			toggleRedEatenFlag();    
			break;
		case PINK:   
			togglePinkEatenFlag();  
			break;
		case BLUE:   
			toggleBlueEatenFlag();   
			break;
		case ORANGE: 
			toggleOrangeEatenFlag();
			break;
		default:
			break;
	}
}

bool World::vulnerable(color ghost_color)
{
	switch (ghost_color)
	{
	case RED:
		return m_red_vulnerable;
	case PINK:
		return m_pink_vulnerable;
	case BLUE:
		return m_blue_vulnerable;
	case ORANGE:
		return m_orange_vulnerable;
	default:
		return false;
	}
}

bool World::eaten(color ghost_color)
{
	switch (ghost_color)
	{
	case RED:
		return m_red_eaten;
	case PINK:
		return m_pink_eaten;
	case BLUE:
		return m_blue_eaten;
	case ORANGE:
		return m_orange_eaten;
	default:
		return false;
	}
}

bool World::isWall(int y, int x, bool fence_on)
{
	if (m_map[y][x] == '#')
		return true;

	if (fence_on && m_map[y][x] == '-')
		return true;

	return false;
}

bool World::isWall(position pos, bool fence_on)
{
	int y = static_cast<int>(round(pos.y));
	int x = static_cast<int>(round(pos.x));

	return isWall(y, x, fence_on);
}

void World::calculatePoints()
{
	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 23; j++)
		{
			if (m_map[i][j] == '*')
				m_points++;
			else if (m_map[i][j] == '@')
				m_big_points++;
		}
	}
}

void World::printMap()
{
	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 23; j++)
			std::cout << m_map[i][j];

		std::cout << '\n';
	}
}

void World::setGhostBuffer(char c, int index) 
{
	if (c == '*' || c == '@' || c == '-')
		m_ghost_buffer[index] = c;
	else
		m_ghost_buffer[index] = ' ';
}
