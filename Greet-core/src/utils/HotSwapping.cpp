#include "HotSwapping.h"

#include <logging/Log.h>

namespace Greet
{
  std::map<uint32_t, Resource*> HotSwapping::resources;

  void HotSwapping::AddHotswapResource(Resource* res)
  {
    Log::Info("Adding hotswapped resource: count=%s, file=%s", resources.size() + 1, res->filePath);
    resources.emplace(res->id, res);
  }

  void HotSwapping::RemoveHotswapResource(Resource* res)
  {
    Log::Info("Removing hotswapped resource: count=%s, file=%s", resources.size() - 1, res->filePath);
    ASSERT(resources.erase(res->id) == 1, "Invalid remove size on Hotswapped resource");
  }

  void HotSwapping::CheckResources()
  {
    for(auto&& resource : resources)
    {
      resource.second->CheckResource();
    }
  }

  void HotSwapping::Destroy()
  {
    if(resources.size() > 0)
    {
      if(resources.size() == 1)
        Log::Warning("1 hotswapped resource was not freed");
      else
        Log::Warning("%s hotswapped resources were not freed", resources.size());
    }
  }
}
