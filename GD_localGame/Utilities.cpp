#include "Utilities.hpp"
#include <iostream>

sf::Vector2f unit(sf::Vector2f vector)
{
	sf::Vector2f unit_vector(0.f, 0.f);
	
	//avoid division by 0
	if (vector== unit_vector) return vector;
	
	float size = sqrt(vector.x * vector.x + vector.y * vector.y);
	unit_vector.x = vector.x / size;
	unit_vector.y = vector.y / size;
	return unit_vector;
}