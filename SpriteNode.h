//
// Created by Anton on 18/12/2021.
//

#ifndef PVPARENA_SPRITENODE_H
#define PVPARENA_SPRITENODE_H

#include "SceneNode.h"

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);
private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite sprite;
};


#endif //PVPARENA_SPRITENODE_H
