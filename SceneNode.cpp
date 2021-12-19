//
// Created by Anton on 28/11/2021.
//
#include "SceneNode.h"

SceneNode::SceneNode(Category::Type category) : children(), parent(nullptr), defaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->parent = this;
	children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::removeChild(const SceneNode &node)
{
	auto found = std::find_if(children.begin(), children.end(),
	                          [&](Ptr &p) -> bool { return p.get() == &node; });
	Ptr result = std::move(*found);
	result->parent = nullptr;
	children.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	drawCurrent(target, states);
	for(const Ptr& child : children)
	{
		child->draw(target, states);
	}
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
}

void SceneNode::update(sf::Time deltaTime, CommandQueue& commands)
{
	updateCurrent(deltaTime, commands);
	updateChildren(deltaTime, commands);
}

void SceneNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
}

void SceneNode::updateChildren(sf::Time deltaTime, CommandQueue& commands)
{
	for(const Ptr& child : children)
		child->update(deltaTime, commands);
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for(const SceneNode *node = this; node != nullptr; node = node->parent)
		transform = node->getTransform() * transform;
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

unsigned int SceneNode::getCategory() const
{
	return defaultCategory;
}

void SceneNode::onCommand(const Command& command, sf::Time deltaTime)
{
	unsigned int category = getCategory();
	if(command.category & category)
		command.action(*this, deltaTime);
	for(auto& child : children)
		child->onCommand(command, deltaTime);
}

sf::FloatRect SceneNode::getBounds() const
{
	return {};
}

void SceneNode::attachToParent(SceneNode::Ptr node)
{
	parent->attachChild(std::move(node));
}

bool SceneNode::collided(const SceneNode &node1, const SceneNode &node2)
{
	return node1.getBounds().intersects(node2.getBounds());
}
