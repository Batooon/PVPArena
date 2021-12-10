//
// Created by Anton on 10/12/2021.
//

#ifndef PVPARENA_DATA_H
#define PVPARENA_DATA_H
#include "Resources.h"
#include <vector>

class Soldier;

struct PlayerData
{
	int Health;
	float Speed;
	Textures::ID Texture;
};

std::vector<PlayerData> initializePlayerData();

#endif //PVPARENA_DATA_H
