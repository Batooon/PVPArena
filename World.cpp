//
// Created by Anton on 28/11/2021.
//

#include "World.h"

World::World(sf::RenderWindow &window):
		window(window),
		worldView(window.getDefaultView()),
		worldBounds(0.f, 0.f, worldView.getSize().x*3, worldView.getSize().y*3),
		spawnPoint(worldBounds.width/2.f, worldBounds.height/2.f),
		player(nullptr),
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

	while(commandQueue.isEmpty()==false)
		sceneGraph.onCommand(commandQueue.pop(), deltaTime);

	sceneGraph.update(deltaTime);
}

CommandQueue &World::getCommandQueue()
{
	return commandQueue;
}
