#include "ECSManager.h"

namespace Greet
{
  ECSManager::~ECSManager()
  {
    for(auto&& components : componentPool)
    {
      delete components.second;
    }
    componentPool.clear();
  }

  EntityID ECSManager::CreateEntity()
  {
    ASSERT(currentEntityId != (uint32_t)-1, "No more entities available");
    entities.emplace(currentEntityId);
    currentEntityId++;
    return currentEntityId-1;
  }

  void ECSManager::DestroyEntity(EntityID entity)
  {
    auto it = entities.find(entity);
    ASSERT(it != entities.end(), "Entity does not exist in ECSManager (entity=", entity, ")");
    entities.erase(it);
    for(auto&& pool : componentPool)
    {
      pool.second->Erase(entity);
    }
  }

  bool ECSManager::ValidEntity(EntityID entity)
  {
    return entities.find(entity) != entities.end();
  }

  void ECSManager::Each(std::function<void(EntityID)> function)
  {
    for(auto e : entities)
      function(e);
  }
}
