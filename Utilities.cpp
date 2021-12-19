//
// Created by Anton on 19/12/2021.
//

#include "Utilities.h"

sf::Vector2f Normalize(const sf::Vector2f& v)
{
	float length = sqrtf(v.x * v.x + v.y * v.y);
	return sf::Vector2f(v.x/length, v.y/length);
}
