//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_ENTITY_H
#define PVPARENA_ENTITY_H

#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

class Entity : public SceneNode
{
public:
	explicit Entity(const sf::Texture& texture);
	Entity(const sf::Texture& texture, const sf::IntRect& rect);

protected:
	sf::Sprite sprite;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //PVPARENA_ENTITY_H
