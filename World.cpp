//
// Created by Anton on 28/11/2021.
//

#include "World.h"

World::World(sf::RenderWindow &window):
		window(window),
		worldView(window.getDefaultView()),
		worldBounds(0.f, 0.f, worldView.getSize().x * 3, worldView.getSize().y * 3),
		spawnPoint(worldBounds.width / 2.f, worldBounds.height / 2.f),
		playerSoldier(nullptr),
		commandQueue()
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

	std::unique_ptr<Soldier> newPlayer(new Soldier(Soldier::MagmaGun, textures));
	playerSoldier = newPlayer.get();
	playerSoldier->setPosition(spawnPoint);
	sceneLayers[Ground]->attachChild(std::move(newPlayer));
}

void World::draw()
{
	window.setView(worldView);
	window.draw(sceneGraph);
}

void World::update(sf::Time deltaTime)
{
	playerSoldier->LookAt(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	playerSoldier->setVelocity(0.f, 0.f);

	while(commandQueue.isEmpty() == false)
		sceneGraph.onCommand(commandQueue.pop(), deltaTime);

	sf::Vector2f velocity = playerSoldier->getVelocity();
	if(velocity.x != 0.f && velocity.y != 0.f)
		playerSoldier->setVelocity(velocity / std::sqrt(2.f));

	sf::Vector2f positionDelta = getViewDeltaPosition(deltaTime);
	worldView.move(positionDelta);

	sceneGraph.update(deltaTime);
	clampPlayerPosition();
	clampWorldView();
}

CommandQueue &World::getCommandQueue()
{
	return commandQueue;
}

void World::clampWorldView()
{
	sf::FloatRect viewRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
	sf::Vector2f viewPosition(viewRect.left, viewRect.top);

	viewPosition.x = std::max(viewPosition.x, worldBounds.left);
	viewPosition.x = std::min(viewPosition.x, worldBounds.width - viewRect.width);
	viewPosition.y = std::max(viewPosition.y, worldBounds.top);
	viewPosition.y = std::min(viewPosition.y, worldBounds.height - viewRect.height);
	worldView.setCenter(viewPosition + worldView.getSize() / 2.f);
}

void World::clampPlayerPosition()
{
	sf::Vector2f position(playerSoldier->getPosition());
	sf::FloatRect playerBounds = playerSoldier->getLocalBounds();

	position.x = std::max(position.x, worldBounds.left);
	position.x = std::min(position.x, worldBounds.width - playerBounds.width);
	position.y = std::max(position.y, worldBounds.top);
	position.y = std::min(position.y, worldBounds.height - playerBounds.height);
	playerSoldier->setPosition(position);
}

sf::Vector2f World::getViewDeltaPosition(sf::Time deltaTime)
{
	sf::FloatRect viewRect(worldView.getCenter(), worldView.getSize());
	sf::Vector2f viewPosition(viewRect.left, viewRect.top);
	sf::Vector2f positionDelta = playerSoldier->getVelocity() * deltaTime.asSeconds();
	sf::Vector2f playerWorldPosition = playerSoldier->getPosition();
	if(playerWorldPosition.x - viewPosition.x >= 0.001f)
		positionDelta.x = 0;
	if(playerWorldPosition.y - viewPosition.y >= 0.001f)
		positionDelta.y = 0;
	return positionDelta;
}
