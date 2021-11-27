#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

template<class VectorT>
sf::Vector2f Normalize(VectorT vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if(length != 0)
		return sf::Vector2f(vector.x / length, vector.y / length);
	return (sf::Vector2f)vector;
}

namespace Textures
{
	enum ID{Landscape,
			DoubleGun,
			LaserGun,
			MagmaGun};
}

enum Layers
{
	Background,
	Ground,
	LayerCount
};

template<class Resource, class Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename);
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

template<class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource());
	if(resource->loadFromFile(filename) == false)
	{
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}
	auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
}

template<class Resource, class Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto resource=resourceMap.find(id);
	return *resource->second;
}

template<class Resource, class Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto texture = resourceMap.find(id);
	return *texture->second;
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	SceneNode();
	void attachChild(Ptr child);
	Ptr removeChild(const SceneNode& node);
	void update(sf::Time deltaTime);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime);
	void updateChildren(sf::Time deltaTime);

	std::vector<Ptr> children;
	SceneNode* parent;
};

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

void SceneNode::update(sf::Time deltaTime)
{
	updateCurrent(deltaTime);
	updateChildren(deltaTime);
}

void SceneNode::updateCurrent(sf::Time deltaTime)
{
}

void SceneNode::updateChildren(sf::Time deltaTime)
{
	for(const auto& child : children)
		child->update(deltaTime);
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

Entity::Entity(const sf::Texture& texture) : sprite(texture)
{
//	sprite.setTexture(texture);
//	sf::FloatRect bounds = sprite.getLocalBounds();
//	if(centerOrigin)
//		sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

Entity::Entity(const sf::Texture& texture, const sf::IntRect& rect) : sprite(texture, rect)
{
}

void Entity::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

class ActiveEntity : public Entity
{
public:
	explicit ActiveEntity(const sf::Texture& texture);
	ActiveEntity(const sf::Texture& texture, const sf::IntRect& rect);
	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float x, float y);
	sf::Vector2f getVelocity() const;

private:
	virtual void updateCurrent(sf::Time deltaTime) override;
	sf::Vector2f velocity;
};

ActiveEntity::ActiveEntity(const sf::Texture& texture) : Entity(texture)
{
}

ActiveEntity::ActiveEntity(const sf::Texture& texture, const sf::IntRect& rect) : Entity(texture, rect)
{
}

void ActiveEntity::setVelocity(float x, float y)
{
	velocity.x=x;
	velocity.y=y;
}

void ActiveEntity::setVelocity(sf::Vector2f velocity)
{
	this->velocity.x=velocity.x;
	this->velocity.y=velocity.y;
}

sf::Vector2f ActiveEntity::getVelocity() const
{
	return velocity;
}

void ActiveEntity::updateCurrent(sf::Time deltaTime)
{
	move(velocity * deltaTime.asSeconds());
}

class Player : public ActiveEntity
{
public:
	enum Type
	{
		DoubleGun,
		LaserGun,
		MagmaGun
	};

	Player(Type type, const TextureHolder& textureHolder);
private:
	const float playerSpeed=250.f;
	Type playerType;
};

Textures::ID toTextureID(Player::Type type)
{
	switch(type)
	{
		case Player::DoubleGun:
			return Textures::DoubleGun;
		case Player::LaserGun:
			return Textures::LaserGun;
		case Player::MagmaGun:
			return Textures::MagmaGun;
	}
}

Player::Player(Type type, const TextureHolder& textureHolder) :
ActiveEntity(textureHolder.get(toTextureID(type))),
playerType(type)
{
	sf::FloatRect localBounds = sprite.getLocalBounds();
	this->setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
}

class Game
{
public:
	Game();

	void run();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	void handlePlayerRotation(sf::Vector2<int> mousePosition);

private:
	void processEvents();

	void update(sf::Time deltaTime);

	void render();

	std::array<SceneNode*, LayerCount> layers;

	SceneNode sceneBaseNode;

	sf::RenderWindow window;
	sf::FloatRect worldBounds;
	TextureHolder textureHolder;
	Player* player;
	float playerSpeed = 250.f;
	const sf::Time TimePerFrame = sf::seconds(1.f / 120.f);

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
};

Game::Game() : window(sf::VideoMode(1920, 1080), "PvP Arena"),
textureHolder(),
player(nullptr),
layers(),
sceneBaseNode(),
worldBounds(0.f,0.f,window.getDefaultView().getSize().x, window.getDefaultView().getSize().y)
{
	textureHolder.load(Textures::DoubleGun, "tanknsoldier/enemy/enemy 2/idle/enemy2idle1.png");
	textureHolder.load(Textures::MagmaGun, "tanknsoldier/enemy/enemy 3/idle/enemy3idle1.png");
	textureHolder.load(Textures::LaserGun, "tanknsoldier/enemy/enemy 1/idle/enemy1idle1.png");
	textureHolder.load(Textures::Landscape, "tanknsoldier/bg/bg2.png");

	for(size_t i = 0; i < LayerCount; i++)
	{
		SceneNode::Ptr layer(new SceneNode());
		layers[i] = layer.get();

		sceneBaseNode.attachChild(std::move(layer));
	}

	sf::Texture& background=textureHolder.get(Textures::Landscape);
	sf::IntRect backgroundRect(worldBounds);
	background.setRepeated(true);

	std::unique_ptr<Entity> backgroundSprite(new Entity(background, backgroundRect));
	layers[Background]->attachChild(move(backgroundSprite));

	std::unique_ptr<Player> newPlayer(new Player(Player::MagmaGun, textureHolder));
	player = newPlayer.get();
	player->setPosition(100.f, 100.f);
	layers[Ground]->attachChild(std::move(newPlayer));
}

void Game::run()
{
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while(window.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while(timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		render();
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if(key == sf::Keyboard::W)
		movingUp = isPressed;
	else if(key == sf::Keyboard::S)
		movingDown = isPressed;
	else if(key == sf::Keyboard::A)
		movingLeft = isPressed;
	else if(key == sf::Keyboard::D)
		movingRight = isPressed;
}

void Game::handlePlayerRotation(sf::Vector2i mousePosition)
{
	sf::Vector2f playerPosition = player->getPosition();
	sf::Vector2f facing(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
		player->setRotation(angle);
}

void Game::processEvents()
{
	sf::Event event{};
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;
			case sf::Event::Closed:
				window.close();
				break;
		}
	}
}

void Game::update(sf::Time deltaTime)
{
	handlePlayerRotation(sf::Mouse::getPosition());
	sf::Vector2i direction(0, 0);
	if(movingUp)
		direction.y = -1;
	if(movingDown)
		direction.y = 1;
	if(movingLeft)
		direction.x = -1;
	if(movingRight)
		direction.x = 1;
	sf::Vector2f velocity = Normalize(direction) * playerSpeed;
	player->setVelocity(velocity);
	player->move(velocity * deltaTime.asSeconds());
}

void Game::render()
{
	window.clear();
	window.draw(sceneBaseNode);
	window.display();
}

int main()
{
	Game game;
	game.run();

	return 0;
}
