#pragma once

#include "GameObject.h"

class Pacman : public GameObject
{
public:
	Pacman(Sprite* sprite)
		: GameObject(sprite, 0.175f, { 11.0f, 20.0f })
	{
	}

	void addLife() { m_lives++; }
	void removeLife() { m_lives--; }

	void updatePos(Direction dir, World& world);
	void validatePos(position next_pos, World& world);
	void checkCollision(World& world, int y, int x);
	void addToScore(int points) { m_score += points; }
	void setDead();
	void strafeSprite(int pixels = 32);
	void reset();

	int getScore() const { return m_score; }
	int getLives() const { return m_lives; }
	bool isDead() const { return m_dead; }

private:
	int m_score{ 0 };
	int m_lives{ 3 };

	bool m_dead{ false };
};
