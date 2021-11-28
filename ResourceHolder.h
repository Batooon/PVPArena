//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_RESOURCEHOLDER_H
#define PVPARENA_RESOURCEHOLDER_H

#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Resources.h"

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

#include "ResourceHolder.inl"
#endif //PVPARENA_RESOURCEHOLDER_H
