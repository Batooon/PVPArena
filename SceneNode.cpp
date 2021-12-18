//
// Created by Anton on 28/11/2021.
//
#include "SceneNode.h"

SceneNode::SceneNode() : children(), parent(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->parent=this;
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
	for(const auto& it : children)
	{
		it->draw(target, states);
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
	for(const auto& child : children)
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
	return Category::Scene;
}

void SceneNode::onCommand(const Command& command, sf::Time deltaTime)
{
	if(command.category & getCategory())
		command.action(*this, deltaTime);
	for(auto& child : children)
		child->onCommand(command, deltaTime);
}