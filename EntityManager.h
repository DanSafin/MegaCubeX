#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <unordered_set>
#include <vector>
#include <iostream>

using Entity = std::size_t;

class EntityManager
{
private:
	Entity nextID = 0;
	std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<void>>> components;

public:
	Entity createEntity()
	{
		return nextID++;
	}

	template<typename T>
	void addComponent(Entity e, const T& component)
	{
		components[typeid(T)][e] = std::make_shared<T>(component);
	}

	template<typename T>
	T* getComponent(Entity e)
	{
		auto it = components.find(typeid(T));
		if (it != components.end() && it->second.count(e))
		{
			return static_cast<T*>(it->second[e].get());
		}
		return nullptr;
	}

	template<typename T>
	std::unordered_map<Entity, std::shared_ptr<void>>& getAllComponents()
	{
		return components[typeid(T)];
	}

	template<typename T>
	bool hasComponent(Entity e)
	{
		auto it = components.find(typeid(T));
		return it != components.end() && it->second.count(e);
	}
	
	template<typename... Components>
	std::vector<Entity> getEntitiesWith()
	{
		std::vector<Entity> result;
		for (const auto& [type, entities_map] : components)
		{
			for (const auto& [entity, _] : entities_map)
			{
				if ((hasComponent<Components>(entity) && ...))
				{
					result.push_back(entity);
				}
			}
		}
		return result;
	}

	void destroyEntity(Entity e)
	{
		for (auto& [type, entities_map] : components)
		{
			entities_map.erase(e);
		}
	}
};