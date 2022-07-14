#include "UUIDComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  UUIDComponent::UUIDComponent(const UUID& uuid)
    : uuid{uuid}
  {
  }

  UUIDComponent::UUIDComponent(const MetaFileClass& metaClass)
    : uuid{MetaFileLoading::LoadString(metaClass,"uuid", UUID{}.GetString())}
  {}

  MetaFile& operator<<(MetaFile& metaFile, const UUIDComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("uuid", component.uuid.GetString());
    metaFile.AddMetaClass("UUIDComponent", meta);
    return metaFile;
  }
}
