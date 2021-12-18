//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_SCENENODE_H
#define PVPARENA_SCENENODE_H

#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Command.h"
#include "Category.h"
#include "CommandQueue.h"

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	SceneNode();
	void attachChild(Ptr child);
	Ptr removeChild(const SceneNode& node);
	void update(sf::Time deltaTime, CommandQueue& commands);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;
	virtual unsigned int getCategory() const;
	void onCommand(const Command& command, sf::Time deltaTime);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	void updateChildren(sf::Time deltaTime, CommandQueue& commands);

	std::vector<Ptr> children;
	SceneNode* parent;
};

#endif //PVPARENA_SCENENODE_H
