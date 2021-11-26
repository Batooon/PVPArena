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
	enum ID{Landscape, Player};
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
	auto resource=resourceMap.find(id);
	return *resource->second;
}

template<class Resource, class Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto texture = resourceMap.find(id);
	return *texture->second;
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

	sf::RenderWindow window;
	ResourceHolder<sf::Texture, Textures::ID> textureHolder;
	sf::Sprite player;
	float playerSpeed = 250.f;
	const sf::Time TimePerFrame = sf::seconds(1.f / 120.f);

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
};

Game::Game() : window(sf::VideoMode(1920, 1080), "SFML"), player()
{
	textureHolder.load(Textures::Player, "tanknsoldier/enemy/enemy 1/idle/enemy1idle1.png");

	player.setTexture(textureHolder.get(Textures::Player));
	player.setOrigin(player.getLocalBounds().width / 2.f, player.getLocalBounds().height / 2.f);
	player.setPosition(100.f, 100.f);
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
	sf::Vector2f playerPosition = player.getPosition();
	sf::Vector2f facing(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
		player.setRotation(angle);
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
	sf::Vector2f movement;
	if(movingUp)
		direction.y = -1;
	if(movingDown)
		direction.y = 1;
	if(movingLeft)
		direction.x = -1;
	if(movingRight)
		direction.x = 1;
	movement = Normalize(direction) * playerSpeed;
	player.move(movement * deltaTime.asSeconds());
}

void Game::render()
{
	window.clear();
	window.draw(player);
	window.display();
}

int main()
{
	Game game;
	game.run();

	return 0;
}
