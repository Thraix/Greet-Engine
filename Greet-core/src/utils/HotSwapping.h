#pragma once

#include <utils/Resource.h>

#include <map>

namespace Greet
{
  struct HotSwapping
  {
    DELETE_CONSTRUCTOR_DESTRUCTOR(HotSwapping);

    private:
      static std::map<uint32_t, Resource*> resources;

    public:
      static void AddHotswapResource(Resource* res);
      static void RemoveHotswapResource(Resource* res);
      static void CheckResources();

      static void Destroy();
  };
}
