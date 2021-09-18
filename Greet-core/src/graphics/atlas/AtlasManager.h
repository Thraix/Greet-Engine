#pragma once

#include <common/Memory.h>
#include <graphics/atlas/Atlas.h>

#include <map>

namespace Greet {
  class AtlasManager
  {
    private:
      static std::map<std::string, Ref<Atlas>> m_atlas;
    public:
      static void Add(const std::string& name, const Ref<Atlas>& atlas);
      static Ref<Atlas>& Get(const std::string& atlasname);
      static void Destroy();
    private:
      AtlasManager(){}
  };
}
