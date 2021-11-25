#include <iostream>
#include <SFML/Graphics.hpp>

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
	sf::CircleShape player;
	float playerSpeed = 50.f;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

	bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;
};

Game::Game():window(sf::VideoMode(640, 480), "SFML"), player()
{
	player.setRadius(40.f);
	player.setPosition(100.f, 100.f);
	player.setFillColor(sf::Color::Cyan);
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
	sf::Event event;
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
	sf::Vector2f movement(0.f, 0.f);
	if(movingUp)
		movement.y -= playerSpeed;
	if(movingDown)
		movement.y += playerSpeed;
	if(movingLeft)
		movement.x -= playerSpeed;
	if(movingRight)
		movement.x += playerSpeed;
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
