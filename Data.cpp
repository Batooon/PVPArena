//
// Created by Anton on 10/12/2021.
//

#include "Data.h"
#include "Soldier.h"

std::vector<PlayerData> initializePlayerData()
{
	std::vector<PlayerData> data(Soldier::Count);

	data[Soldier::DoubleGun].Health = 150;
	data[Soldier::DoubleGun].Speed = 200.f;
	data[Soldier::DoubleGun].Texture = Textures::DoubleGun;

	data[Soldier::LaserGun].Health = 100;
	data[Soldier::LaserGun].Speed = 300.f;
	data[Soldier::LaserGun].Texture = Textures::LaserGun;

	data[Soldier::MagmaGun].Health = 100;
	data[Soldier::MagmaGun].Speed = 250.f;
	data[Soldier::MagmaGun].Texture = Textures::MagmaGun;

	return data;
}

