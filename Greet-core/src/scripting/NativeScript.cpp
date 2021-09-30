#include "NativeScript.h"

#include <ecs/components/TagComponent.h>

namespace Greet
{
  Entity NativeScript::FindEntityByTag(const std::string& tag)
  {
    if(entity)
    {
      ECSManager* manager = entity.GetManager();
      EntityID ret = 0;

      manager->Each<Greet::TagComponent>(
        [&](EntityID tagEntity, Greet::TagComponent& tagComponent)
        {
          if(tagComponent.tag == tag)
            ret = tagEntity;
        }
      );

      if(ret)
        return Entity{entity.GetManager(), ret};
    }
    return Entity{entity.GetManager()};
  }
}
