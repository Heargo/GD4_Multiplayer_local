// HUGO REY D00262075 : changes to cap the velocity of the entity and add friction to the entity

#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints);
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	sf::Vector2f GetVelocity() const;
	void Accelerate(sf::Vector2f velocity);
	void Accelerate(float vx, float vy);

	int GetHitPoints() const;
	void Repair(unsigned int points);
	void Damage(unsigned int points);
	void ApplyFriction();
	void Destroy();
	virtual bool IsDestroyed() const;

private:
	virtual void UpdateCurrent(sf::Time dt);

private:
	sf::Vector2f m_velocity;
	unsigned int m_hitpoints;
};

