#pragma once

#include <vector>

#include <logging/Log.h>
#include <ecs/ComponentPool.h>

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <typeindex>

namespace Greet
{
  class ECSManager final
  {
    private:
      std::unordered_set<EntityID> entities;
      std::unordered_map<std::type_index, ComponentPoolBase*> componentPool;
      int currentEntityId = 1;
    public:
      ~ECSManager();

      EntityID CreateEntity();
      void DestroyEntity(EntityID entity);
      size_t GetEntityCount() const { return entities.size(); }
      bool ValidEntity(EntityID entity);
      void Each(std::function<void(EntityID)> function);

      template <typename... Components, typename... Args>
      std::tuple<Components&...> AddComponents(EntityID entity, Components&&... components)
      {
        return std::forward_as_tuple(AddComponent(entity, Components(components))...);
      }

      template <typename Component, typename... Args>
      Component& AddComponent(EntityID entity, Args&&... args)
      {
        return AddComponent(entity, Component(args...));
      }

      template <typename Component>
      Component& AddComponent(EntityID entity, const Component& component)
      {
        auto pool = GetComponentPool<Component>();

        if(pool)
        {
          ASSERT(!HasComponent<Component>(entity), "Component already exists in entity (entity=", entity, ", Component=", typeid(Component).name(), ")")
          return pool->Emplace(entity, component);
        }
        else
        {
          ComponentPool<Component>* pool{new ComponentPool{entity, component}};
          auto ret = componentPool.emplace(GetComponentId<Component>(), pool);
          return pool->At(0);
        }
      }

      template <typename Component>
      void RemoveComponent(EntityID entity)
      {
        auto pool = GetComponentPoolAssure<Component>();
        ASSERT(pool->Erase(entity), "Entity did not contain component (entity=", entity, ", Component=", typeid(Component).name(), ")")
      }

      template <typename... Components>
      void RemoveComponents(EntityID entity)
      {
        (RemoveComponent<Components>(entity), ...);
      }

      template <typename Component>
      Component& GetComponent(EntityID entity)
      {
        auto pool = GetComponentPoolAssure<Component>();
        Component* component = pool->FindComponent(entity);
        ASSERT(component, "Entity did not contain component (entity=", entity, ", Component=", typeid(Component).name(), ")")
        return *component;
      }

      template <typename Component>
      bool HasComponent(EntityID entity)
      {
        auto pool = GetComponentPool<Component>();
        if(pool)
          return pool->Find(entity) != pool->Size();
        return false;
      }

      template <typename... Components>
      bool HasComponents(EntityID entity)
      {
        return (HasComponent<Components>(entity) && ...);
      }

      template <typename Component, typename... Components, typename Func>
      void Each(Func function)
      {
        auto pool = GetComponentPool<Component>();
        if(pool)
        {
          size_t i = 0;
          for(auto entity : pool->GetEntities())
          {
            if(HasComponents<Components...>(entity))
            {
              std::apply(function, std::forward_as_tuple(entity, pool->At(i), GetComponent<Components>(entity)...));
            }
            i++;
          }
        }
      }

      template <typename Component>
      void Each(std::function<void(EntityID, Component&)> function)
      {
        auto pool = GetComponentPool<Component>();
        if(pool)
        {
          size_t i = 0;
          for(auto e : pool->GetEntities())
          {
            function(e, pool->At(i));
            i++;
          }
        }
      }

      template <typename Component, typename... Components, typename Func>
      EntityID Find(Func function)
      {
        auto pool = GetComponentPool<Component>();
        if(pool)
        {
          size_t i = 0;
          for(auto entity : pool->GetEntities())
          {
            if(HasComponents<Components...>(entity))
            {
              if(std::apply(function, std::forward_as_tuple(entity, pool->At(i), GetComponent<Components>(entity)...)))
                return entity;
            }
            i++;
          }
        }
        return 0;
      }

      template <typename Component>
      EntityID Find(std::function<bool(EntityID, Component&)> function)
      {
        auto pool = GetComponentPool<Component>();
        if(pool)
        {
          size_t i = 0;
          for(auto e : pool->GetEntities())
          {
            if(function(e, pool->At(i)))
              return e;
            i++;
          }
        }
        return 0;
      }

      template <typename T>
      std::type_index GetComponentId()
      {
        return std::type_index(typeid(T));
      }

    private:
      template <typename Component>
      ComponentPool<Component>* GetComponentPool()
      {
        auto it = componentPool.find(GetComponentId<Component>());
        return it == componentPool.end() ? nullptr : static_cast<ComponentPool<Component>*>(it->second);
      }

      template <typename Component>
      ComponentPool<Component>* GetComponentPoolAssure()
      {
        auto it = componentPool.find(GetComponentId<Component>());
        ASSERT(it != componentPool.end(), "Component has not been added to an entity (Component=", typeid(Component).name(), ")");
        return static_cast<ComponentPool<Component>*>(it->second);
      }
  };
}
