//
// Created by Anton on 28/11/2021.
//

#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"

Entity::Entity(const sf::Texture& texture) : sprite(texture)
{
}

Entity::Entity(const sf::Texture& texture, const sf::IntRect& rect) : sprite(texture, rect)
{
}

void Entity::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}