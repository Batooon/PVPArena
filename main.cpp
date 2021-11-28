#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

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
			Tree,
			Leafs,
			DoubleGun,
			LaserGun,
			MagmaGun};
}

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
	auto resource = resourceMap.find(id);
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

	void LookAt(sf::Vector2f worldPosition);
	float getSpeed() const;

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
private:
	const float speed = 250.f;
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

void Player::LookAt(sf::Vector2f worldPosition)
{
	sf::Vector2f playerPosition = this->getPosition();
	sf::Vector2f facing(worldPosition.x - playerPosition.x, worldPosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
	this->setRotation(angle);
}

float Player::getSpeed() const
{
	return speed;
}

class World:sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw();

private:
	enum Layers
	{
		Background,
		Vegetation,
		Trees,
		Ground,
		LayerCount
	};

	void loadTextures();
	void buildScene();

	sf::RenderWindow& window;
	sf::View worldView;
	TextureHolder textures;
	SceneNode sceneGraph;
	std::array<SceneNode*, LayerCount> sceneLayers;

	sf::FloatRect worldBounds;
	sf::Vector2f spawnPoint;
	float camerafollowSpeed;
	Player* player;
};

class Game
{
public:
	Game();

	void run();

private:
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

	sf::RenderWindow window;
	World world;
	static const sf::Time TimePerFrame;

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
};

const sf::Time Game::TimePerFrame = sf::seconds(1.f/120.f);

Game::Game() :
window(sf::VideoMode(1920, 1080), "PvP Arena", sf::Style::Close), world(window)
{
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
	world.update(deltaTime);
}

void Game::render()
{
	window.clear();
	world.draw();
	window.display();
}

World::World(sf::RenderWindow &window):
window(window),
worldView(window.getDefaultView()),
worldBounds(0.f, 0.f, worldView.getSize().x*3, worldView.getSize().y*3),
spawnPoint(worldBounds.width/2.f, worldBounds.height/2.f),
player(nullptr)
{
	loadTextures();
	buildScene();

	worldView.setCenter(spawnPoint);
}

void World::loadTextures()
{
	textures.load(Textures::DoubleGun, "tanknsoldier/enemy/enemy 2/idle/enemy2idle1.png");
	textures.load(Textures::MagmaGun, "tanknsoldier/enemy/enemy 3/idle/enemy3idle1.png");
	textures.load(Textures::LaserGun, "tanknsoldier/enemy/enemy 1/idle/enemy1idle1.png");
	textures.load(Textures::Landscape, "KTT Series - Plant Biome/Grass.png");
	textures.load(Textures::Leafs, "KTT Series - Plant Biome/GrassWithLeafs.png");
	textures.load(Textures::Tree, "KTT Series - Plant Biome/Tree.png");
}

void World::buildScene()
{
	for(size_t i = 0; i < LayerCount; i++)
	{
		SceneNode::Ptr layer(new SceneNode());
		sceneLayers[i] = layer.get();

		sceneGraph.attachChild(std::move(layer));
	}

	sf::Texture &background = textures.get(Textures::Landscape);
	sf::IntRect backgroundRect(worldBounds);
	background.setRepeated(true);
	std::unique_ptr<Entity> backgroundSprite(new Entity(background, backgroundRect));
	backgroundSprite->setPosition(worldBounds.left, worldBounds.top);
	sceneLayers[Background]->attachChild(move(backgroundSprite));

	sf::Texture &tree = textures.get(Textures::Tree);
	for(int i = 0; i < 150; i++)
	{
		std::unique_ptr<Entity> treeSprite(new Entity(tree));
		sf::Vector2f position((float) rand() / RAND_MAX * (float) worldBounds.width,
		                      (float) rand() / RAND_MAX * (float) worldBounds.height);
		treeSprite->setPosition(position);
		treeSprite->setScale(2.5f, 2.5f);
		sceneLayers[Trees]->attachChild(std::move(treeSprite));
	}

	sf::Texture &plant = textures.get(Textures::Leafs);
	for(int i = 0; i < 250; i++)
	{
		std::unique_ptr<Entity> plantSprite(new Entity(plant));
		sf::Vector2f position((float) rand() / RAND_MAX * (float) worldBounds.width,
		                      (float) rand() / RAND_MAX * (float) worldBounds.height);
		plantSprite->setPosition(position);
		plantSprite->setScale(1.5f, 1.5f);
		sceneLayers[Vegetation]->attachChild(std::move(plantSprite));
	}

	std::unique_ptr<Player> newPlayer(new Player(Player::MagmaGun, textures));
	player = newPlayer.get();
	player->setPosition(spawnPoint);
	sceneLayers[Ground]->attachChild(std::move(newPlayer));
}

void World::draw()
{
	window.setView(worldView);
	window.draw(sceneGraph);
}

void World::update(sf::Time deltaTime)
{
	player->LookAt(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
//	sf::Vector2i direction(0, 0);
//	if(player->movingUp)
//		direction.y = -1;
//	if(player->movingDown)
//		direction.y = 1;
//	if(player->movingLeft)
//		direction.x = -1;
//	if(player->movingRight)
//		direction.x = 1;
//	sf::Vector2f velocity = Normalize(direction) * player->getSpeed();
//	player->setVelocity(velocity);
//	player->move(velocity * deltaTime.asSeconds());
//	worldView.move(velocity * deltaTime.asSeconds());

//	sceneGraph.update(deltaTime);
}

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch(std::exception& e)
	{
		std::cout<<"Exception: "<<e.what()<<std::endl;
	}

	return 0;
}
