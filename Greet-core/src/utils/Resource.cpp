#include "Resource.h"

#include <utils/HotSwapping.h>

namespace Greet
{
  uint32_t Resource::s_Id = 0;

  Resource::Resource(const std::string& filePath)
    : id{++s_Id}, modDate{FileUtils::GetTimeModified(filePath)}, filePath{filePath}
  {
#if defined(_DEBUG) && defined(_GREET_HOTSWAP)
    HotSwapping::AddHotswapResource(this);
#endif
  }

  Resource::~Resource()
  {
#if defined(_DEBUG) && defined(_GREET_HOTSWAP)
    if(id != 0)
      HotSwapping::RemoveHotswapResource(this);
#endif
  }


  void Resource::CheckResource()
  {
    FileUtils::TimeModified newDate = FileUtils::GetTimeModified(filePath);
    if(modDate < newDate)
    {
      modDate = newDate;
      ReloadResource();
    }
  }
}
