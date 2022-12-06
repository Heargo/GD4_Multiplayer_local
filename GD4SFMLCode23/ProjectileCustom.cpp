//HUGO REY D00262075 : add functions from .hpp

#pragma once
#include "ProjectileCustom.hpp"
#include "Projectile.hpp"
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceHolder.hpp"
#include "Texture.hpp"
#include "Entity.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <iostream>


ProjectileCustom::ProjectileCustom(ProjectileCustom::Type type, const TextureHolder& texture, SceneNode* air_layer)
	:Entity(1)
	,m_type(type)
	,m_sprite(texture.Get(Texture::kBullet))
	, m_air_layer(air_layer)
{
	//log to console 
	std::cout << "Projectile created." << std::endl;
}

unsigned int ProjectileCustom::getCategory() const
{
	return 0;
}

float ProjectileCustom::getMaxSpeed() const
{
	return 0.0f;
}

int ProjectileCustom::getDamage() const
{
	return 1;
}

void ProjectileCustom::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	//check if collide with an entity in the air layer
	//if so, destroy the projectile
	m_air_layer->DetectCollisionAndApplyDamage(getPosition(), 1, getDamage());
	

	Entity::UpdateCurrent(dt, commands);
}

void ProjectileCustom::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
