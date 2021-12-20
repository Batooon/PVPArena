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
#include <set>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;
	explicit SceneNode(Category::Type category = Category::None);
	void attachChild(Ptr child);
	Ptr removeChild(const SceneNode& node);
	void update(sf::Time deltaTime, CommandQueue& commands);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;
	virtual unsigned int getCategory() const;
	void onCommand(const Command& command, sf::Time deltaTime);
	virtual sf::FloatRect getBounds() const;
	void attachToParent(Ptr node);
	bool collided(const SceneNode& node1, const SceneNode& node2);
	void checkNodeCollisions(SceneNode& node, std::set<Pair>& collisions);
	void checkSceneCollisions(SceneNode& sceneGraph, std::set<Pair>& collisions);
	virtual bool isDestroyed() const;
	bool needToRemove() const;
	void removeDestroyedNodes();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	void updateChildren(sf::Time deltaTime, CommandQueue& commands);

	std::vector<Ptr> children;
	SceneNode* parent;
	Category::Type defaultCategory = Category::None;
};

#endif //PVPARENA_SCENENODE_H
