//HUGO REY D00262075 : Asteroid are entity of random size in the game, the are slowly rotating.
//to implement : 
//They do damage to the player if they collide with it. The damage is proportional to the velocity of the player.
//The player can destroy them by shooting them with the missiles.

#pragma once
#include "Entity.hpp"
#include "AircraftType.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "TextNode.hpp"

class Asteroid : public Entity
{

public:
	Asteroid(int size, const TextureHolder& textures);

private:
	float m_size;
	sf::Sprite m_sprite;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	
};

