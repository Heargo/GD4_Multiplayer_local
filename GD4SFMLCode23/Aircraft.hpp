#pragma once
#include "Entity.hpp"
#include "AircraftType.hpp"
#include "ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "TextNode.hpp"

#include "Command.hpp"

#include "Projectile.hpp"


#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity
{
public:
	Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts);
	unsigned int GetCategory() const override;

	void IncreaseFireRate();
	void IncreaseFireSpread();
	void CollectMissiles(unsigned int count);
	void UpdateTexts();
	void UpdateMovementPattern(sf::Time dt);
	float GetMaxSpeed() const;
	
	//Added fire and launchMissile
	void fire();
	void launchMissile();

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

	//Added the code below till updateTexts to toggle pickup/bullet/projectile interaction
	void updateMovementPattern(sf::Time dt);
	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void createPickup(SceneNode& node, const TextureHolder& textures) const;

	void updateTexts();
	
private:
	AircraftType m_type;
	sf::Sprite m_sprite;

	unsigned int m_fire_rate;
	unsigned int m_spread_level;
	unsigned int m_missile_ammo;
	TextNode* m_health_display;
	TextNode* m_missile_display;
	float m_travelled_distance;
	int m_directions_index;

	//Added code below till mMissileDisplay to toggle pickup/bullet/projectile interaction n visual
	sf::Sprite mSprite;
	Command m_fire_command;
	Command	m_missile_command;
	sf::Time m_fire_countdown;
	bool m_is_firing;
	bool m_is_launching_missile;
	bool m_is_marked_for_removal;

	int	m_fire_rate_level;
	int	m_spread_level;
	int	m_missile_ammo;

	Command m_drop_pickup_command;
	float	m_travelled_distance;
	TextNode* m_health_display;
	TextNode* m_missile_display;
};

