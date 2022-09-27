#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"

class Game
{
public:
	Game();
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time delta_time);
	void Render();
	void HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed);

private:
	sf::RenderWindow m_window;
	Player m_player;
	static const float kPlayerSpeed;
	static const sf::Time kTimePerFrame;
	bool m_is_moving_up = false;
	bool m_is_moving_down = false;
	bool m_is_moving_left = false;
	bool m_is_moving_right = false;
};

