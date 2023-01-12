//HUGO REY D00262075 : add GetType() to know if aircraft is player 1 or 2
// add Fire() to shoot projectiles
#pragma once

#include "Entity.hpp"
#include "AircraftType.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "TextNode.hpp"
#include "ProjectileCustom.hpp"

class Aircraft : public Entity
{
public:
	Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts, SceneNode* m_air_layer );
	unsigned int GetCategory() const override;

	void IncreaseFireRate();
	void IncreaseFireSpread();
	void CollectMissiles(unsigned int count);
	void UpdateTexts();
	void UpdateMovementPattern(sf::Time dt);
	float GetMaxSpeed() const;
	void Fire();
	void ApplyDamage(float damage);
	bool IsMarkedForRemoval() const;

	AircraftType GetType();


	void PlayLocalSound(CommandQueue& commands, SoundEffect effect);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	sf::Vector2f BulletPosition();

	void CheckProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void PlayLocalSound(CommandQueue& commands, SoundEffect effect);
	
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
	SceneNode* m_air_layer;

	Command m_fire_command;

	bool m_is_firing;
	sf::Time m_fire_countdown;

	bool m_played_explosion_sound;

	const TextureHolder& m_textures;
};
