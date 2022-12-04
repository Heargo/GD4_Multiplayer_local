//HUGO REY D00262075 : fix errors 

#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "TextNode.hpp"
/* old projectileType
enum class ProjectileType
{
	kAlliedBullet,
	kEnemyBullet,
	kMissile,
	kProjectileCount
}; */


class ProjectileType : public Entity
{
public:
	enum Type
	{
		kPlayer1Bullet,
		kPlayer2Bullet,
		kProjectileCount
	};

public:
	ProjectileType(ProjectileType::Type type, const TextureHolder& texture);
	//void guideTowards(sf::Vector2f position);
	//bool isGuided() const;
	virtual unsigned int getCategory() const;
	//virtual sf::FloatRect getBoundingRect const;
	float getMaxSpeed() const;
	int getDamage() const;

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	Type m_type;
	sf::Sprite m_sprite;
	//sf::Vector2f m_targetDirection;
};
