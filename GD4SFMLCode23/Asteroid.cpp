//HUGO REY D00262075 : Create the asteroid class.

#include "Asteroid.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "ResourceHolder.hpp"
#include <iostream>

Asteroid::Asteroid(int size, const TextureHolder& textures)
	:Entity(5*(size / 50))
	, m_size(size)
	, m_sprite(textures.Get(Texture::kAsteroid))
{
	//set origin at the center of the sprite
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	
	//scale the sprite to the size of the asteroid
	m_sprite.setScale(m_size / bounds.width, m_size / bounds.height);
}

unsigned int Asteroid::GetCategory() const
{
	return static_cast<unsigned int>(ReceiverCategories::kAsteroid);
}

int Asteroid::GetRadius()
{
	//get the radius of the sprite taking scale into account
	return static_cast<int>(std::max(m_sprite.getScale().x, m_sprite.getScale().y) * m_sprite.getTexture()->getSize().x / 2);
}

void Asteroid::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{	
	target.draw(m_sprite, states);
}

void Asteroid::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	//Entity::UpdateCurrent(dt, commands);
	m_sprite.rotate(0.05);
}
