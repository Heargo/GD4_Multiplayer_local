// HUGO REY D00262075 : changes to cap the velocity of the entity and add friction to the entity

#include "Entity.hpp"
#include <cassert>

Entity::Entity(int hitpoints)
    : m_hitpoints(hitpoints)
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::SetVelocity(float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f Entity::GetVelocity() const
{
    return m_velocity;
}

void Entity::Accelerate(sf::Vector2f velocity)
{
    m_velocity += velocity;
}

void Entity::Accelerate(float vx, float vy)
{
	//cap to a max speed of 100
	//x direction
	if (m_velocity.x + vx > 100)
		m_velocity.x = 100;
    else
		m_velocity.x += vx;
	
    //y direction
	if (m_velocity.y + vy > 100)
		m_velocity.y = 100;
    else
    m_velocity.y += vy;
}

/**
 * Apply a "friction" to the entity's velocity. This allow the entity to slow down over time if the player do not press any key.
 * This is to apply a "drag" effect in space to the entity and make the controls more challenging.
 * For now the frictionIntensity is set to 0.1f, which means that the entity will slow down by 10% of its current velocity every time this methods is called.
 * @param No parameters
 * @return No return value
 */
void Entity::ApplyFriction()
{
    float frictionIntensity = 0.1f;
	m_velocity.x *= (1.f - frictionIntensity);
	m_velocity.y *= (1.f - frictionIntensity);
}

int Entity::GetHitPoints() const
{
    return m_hitpoints;
}

void Entity::Repair(unsigned int points)
{
    assert(points > 0);
    //TODO Limit hitpoints
    m_hitpoints += points;
}

void Entity::Damage(unsigned int points)
{
    assert(points > 0);
    m_hitpoints -= points;
}

void Entity::Destroy()
{
    m_hitpoints = 0;
}

bool Entity::IsDestroyed() const
{
    return m_hitpoints <=0;
}

void Entity::UpdateCurrent(sf::Time dt)
{
    move(m_velocity * dt.asSeconds());
}
