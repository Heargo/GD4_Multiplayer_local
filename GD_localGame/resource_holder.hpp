#pragma once
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <algorithm>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void Load(Identifier id, const std::string& filename);
	template <typename Parameter>
	void Load(Identifier id, const std::string& filename, const Parameter& secondParm);
	Resource& Get(Identifier id);
	const Resource& Get(Identifier id) const;

private:
	std::map < Identifier, std::unique_ptr<Resource>> m_resource_map;
};

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::Load(Identifier id, const std::string& filename)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("ResourceHolder::Load failed to load " + filename);
    }
    auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template<typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::Get(Identifier id)
{
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
}

template<typename Resource, typename Identifier>
inline const Resource& ResourceHolder<Resource, Identifier>::Get(Identifier id) const
{
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void ResourceHolder<Resource, Identifier>::Load(Identifier id, const std::string& filename, const Parameter& secondParm)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, secondParm))
    {
        throw std::runtime_error("ResourceHolder::Load failed to load " + filename);
    }
    auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
