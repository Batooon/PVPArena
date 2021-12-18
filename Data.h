//
// Created by Anton on 10/12/2021.
//

#ifndef PVPARENA_DATA_H
#define PVPARENA_DATA_H

#include "Resources.h"
#include <vector>
#include "Soldier.h"
#include "Projectile.h"

class Soldier;
class Projectile;

struct PlayerData
{
	int Health;
	float Speed;
	Textures::ID Texture;
};

struct ProjectileData
{
	float Speed;
	int Damage;
	Textures::ID Texture;
};

std::vector<PlayerData> initializePlayerData();
std::vector<ProjectileData> initializeProjectileData();

#endif //PVPARENA_DATA_H
