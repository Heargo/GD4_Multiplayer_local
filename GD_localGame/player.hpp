#include <SFML/Graphics.hpp>
#pragma once
class Player
{
public:
	Player();
	sf::Sprite GetSprite();
	void Move(sf::Vector2f);

private:
	sf::Texture m_texture;
	sf::Sprite m_player;
	sf::Vector2f m_position;
	float m_speed;
};

