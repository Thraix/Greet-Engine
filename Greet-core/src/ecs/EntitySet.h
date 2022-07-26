#pragma once

#include <ecs/Config.h>

#include <stddef.h>
#include <unordered_map>
#include <vector>

namespace Greet
{
  class EntitySet
  {
    private:
      std::vector<EntityID> entitiesList;
      std::unordered_map<EntityID, size_t> entitiesMap; // Maps the entity id to a component index
    public:
      bool Emplace(EntityID entity);
      bool Erase(EntityID entity);
      bool Pop();
      size_t Find(EntityID entity);
      size_t Size() const;
      std::vector<EntityID>& GetList();
      const std::vector<EntityID>& GetList() const;

      std::vector<EntityID>::iterator Begin();
      std::vector<EntityID>::iterator End();
      std::vector<EntityID>::iterator begin();
      std::vector<EntityID>::iterator end();
  };
};
