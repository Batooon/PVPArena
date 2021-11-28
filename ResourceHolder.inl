//
// Created by Anton on 28/11/2021.
//

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