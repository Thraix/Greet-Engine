#pragma once

#include <ecs/Config.h>
#include <ecs/ECSManager.h>

namespace Greet
{
  class Entity
  {
    friend class ECSManager;

    private:
      ECSManager* manager;
      EntityID id;

    public:
      Entity()
        : manager{nullptr}, id{0}
      {}

      Entity(ECSManager* manager)
        : manager{manager}, id{0}
      {}

      Entity(ECSManager* manager, EntityID id)
        : manager{manager}, id{id}
      {}

      operator EntityID() const { return id; }

      void operator=(EntityID entityId)
      {
        id = entityId;
      }

      bool operator==(const Entity& entity)
      {
        return id == entity.id;
      }

      bool operator!=(const Entity& entity)
      {
        return id != entity.id;
      }

      operator bool() const
      {
        if(manager)
          return manager->ValidEntity(id);
        return false;
      }

      void Invalidate()
      {
        id = 0;
      }

      void Destroy()
      {
        if(*this)
          manager->DestroyEntity(id);
      }

      template <typename Component, typename... Args>
      inline Component& AddComponent(Args... args)
      {
        return manager->AddComponent<Component>(id, args...);
      }

      template <typename... Components>
      std::tuple<Components&...> AddComponents(Components&&... components)
      {
        return manager->AddComponents(id, components...);
      }

      template <typename Component>
      inline void RemoveComponent()
      {
        return manager->RemoveComponent<Component>(id);
      }

      template <typename... Components>
      inline void RemoveComponents()
      {
        return manager->RemoveComponents<Components...>(id);
      }

      template <typename Component>
      inline Component& GetComponent() const
      {
        return manager->GetComponent<Component>(id);
      }

      template <typename Component>
      inline bool HasComponent() const
      {
        return manager->HasComponent<Component>(id);
      }

      template <typename... Components>
      inline bool HasComponents() const
      {
        return manager->HasComponents<Components...>(id);
      }

      template <typename... Components>
      inline bool HasAnyComponent() const
      {
        return manager->HasAnyComponent<Components...>(id);
      }

      void SetID(EntityID aId)
      {
        id = aId;
      }

      EntityID GetID() const
      {
        return id;
      }

      ECSManager* GetManager() const
      {
        return manager;
      }

      static Entity Create(ECSManager* manager)
      {
        return {manager, manager->CreateEntity()};
      }
  };
}
