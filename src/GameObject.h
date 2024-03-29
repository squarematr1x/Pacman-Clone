#pragma once

#include <map>

#include "Sprite.h"
#include "World.h"

class GameObject
{
public:
	GameObject(Sprite* sprite, float speed, position pos, Mode mode = Mode::REST)
		: m_sprite(sprite), m_speed(speed), m_pos(pos), m_mode(mode)
	{
	}

	virtual ~GameObject()
	{
		delete m_sprite;
	}

	int lerp(float current_pos, float next_pos);

	float getDistance(position src, position dest);
	float getSpeed() const { return m_speed; }

	position getPos() const { return m_pos; }

	Direction getDir() const { return m_dir; }

	Mode getMode() const { return m_mode; }

	bool atLeftmostPos();
	bool atRightmostPos();
	bool movingVertically();
	bool movingHorizontally();

	virtual void updatePos(Direction dir, World& world) {};

	void setSpeed(float new_speed) { m_speed = new_speed; }
	void setPos(position new_pos) { m_pos = new_pos; }
	void setDir(Direction new_direction) { m_dir = new_direction; }
	void setMode(Mode new_mode) { m_mode = new_mode; }

	void render();

protected:
	Sprite* m_sprite;
	float m_speed;
	position m_pos;
	Mode m_mode;
	Direction m_dir{ Direction::LEFT };
	bool m_changed_dir{ false };
};

class Ghost : public GameObject
{
public:
	Ghost(Sprite* sprite, Mode mode, position scatter_dest, color col)
		: GameObject(sprite, 0.065f, { 12.0f, 13.0f }, mode),
		  m_scatter_dest(scatter_dest), m_color(col)
	{
	}

	virtual ~Ghost()
	{
	}

	virtual void toggleFleeEndingFlag() { m_flee_ending = !m_flee_ending; }

	virtual void updatePos(int x, int y, Direction dir, World& world);
	virtual void updateMapPos(World& world);
	virtual void updateDest(position& dest, position pacman_pos, position red_pos, Direction pacman_dir);
	virtual void update(position pacman_pos, position red_pos, Direction pacman_dir, World& world);
	virtual void changeMode(Mode mode);
	virtual void start(int x, int y, World& world);

	virtual void reset(World& world) = 0;
	virtual void setToEaten(World& world) = 0;
	virtual void setMapTile(World& world, char next_tile, int x, int y) = 0;
	virtual void updateLastPos(int x, int y, World& world) = 0;

	virtual position getChasePos(position pacman_pos, position red_pos, Direction pacman_dir) = 0;

	virtual std::map<Direction, position> getPossiblePos(Direction last_dir);
	virtual std::map<Direction, position> getOppositePos(Direction last_dir);

	color getColor() const { return m_color; }

	bool escaped() const { return m_escaped; }
	bool canEatPlayer();

protected:
	bool m_escaped{ false };
	bool m_reverse_dir{ false };
	bool m_flee_ending{ false };
	bool m_changed_world_pos{false};

	position m_scatter_dest;
	world_position m_world_pos{ 0, 0 };

	color m_color;
};

class Red : public Ghost
{
public:
	Red(Sprite* sprite, position scatter_dest = {21.0f, 0.0f})
		: Ghost(sprite, Mode::REST, scatter_dest, RED)
	{
	}

	void reset(World& world);
	void setToEaten(World& world);
	void setMapTile(World& world, char next_tile, int x, int y);
	void updateLastPos(int x, int y, World& world) { world.setTileAt(y, x, world.getGhostBuffer(0)); };

	position getChasePos(position pacman_pos, position red_pos, Direction pacman_dir);
};

class Pink : public Ghost
{
public:
	Pink(Sprite* sprite, position scatter_dest = {1.0f, 0.0f})
		: Ghost(sprite, Mode::START, scatter_dest, PINK)
	{
	}

	void reset(World& worldp);
	void setToEaten(World& world);
	void setMapTile(World& world, char next_tile, int x, int y);
	void updateLastPos(int x, int y, World& world) { world.setTileAt(y, x, world.getGhostBuffer(1)); };

	position getChasePos(position pacman_pos, position red_pos, Direction pacman_dir);
};

class Blue : public Ghost
{
public:
	Blue(Sprite* sprite, position scatter_dest = { 22.0f, 26.0f })
		: Ghost(sprite, Mode::REST, scatter_dest, BLUE)
	{
	}

	void reset(World& world);
	void setToEaten(World& world);
	void setMapTile(World& world, char next_tile, int x, int y);
	void updateLastPos(int x, int y, World& world) { world.setTileAt(y, x, world.getGhostBuffer(2)); };

	position getChasePos(position pacman_pos, position red_pos, Direction pacman_dir);
};

class Orange : public Ghost
{
public:
	Orange(Sprite* sprite, position scatter_dest = { 0.0f, 26.0f })
		: Ghost(sprite, Mode::REST, scatter_dest, ORANGE)
	{
	}

	void reset(World& world);
	void setToEaten(World& world);
	void setMapTile(World& world, char next_tile, int x, int y);
	void updateLastPos(int x, int y, World& world) { world.setTileAt(y, x, world.getGhostBuffer(3)); };

	position getChasePos(position pacman_pos, position red_pos, Direction pacman_dir);
};
