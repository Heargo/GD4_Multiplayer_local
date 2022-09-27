#include "player.hpp"
#include <iostream>


Player::Player():
	m_texture(), m_player(), m_position(), m_speed()
{
	if (!m_texture.loadFromFile("Media/Textures/EAgle.png"))
	{
		//handle failure
	}
	
	m_player.setTexture(m_texture);

	m_speed = 100.f;
	m_position.x = 300.f;
	m_position.y = 300.f;

	m_player.setPosition(m_position.x, m_position.y);
	
}


sf::Sprite Player::GetSprite()
{
	return m_player;
}

void Player::Move(sf::Vector2f direction)
{
	m_position += direction * m_speed;

	m_player.move(direction * m_speed);
}