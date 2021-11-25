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

class Game
{
public:
	Game();

	void run();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	void processEvents();

	void update(sf::Time deltaTime);

	void render();

	sf::RenderWindow window;
	sf::Sprite player;
	sf::Texture playerTexture;
	float playerSpeed = 250.f;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
};

Game::Game():window(sf::VideoMode(1920, 1080), "SFML"), playerTexture(), player()
{
	if(playerTexture.loadFromFile("tanknsoldier/enemy/enemy 1/idle/enemy1idle1.png") == false)
	{
	}
	player = sf::Sprite(playerTexture);
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
