#pragma once

#include <utils/MetaFile.h>
#include <utils/UUID.h>

namespace Greet
{
  struct UUIDComponent
  {
    UUID uuid;

    UUIDComponent(const UUID& uuid);
    UUIDComponent(const MetaFileClass& metaClass);

    friend MetaFile& operator<<(MetaFile& metaFile, const UUIDComponent& component);
  };
}

