#pragma once

#include <graphics/models/Mesh.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct MeshComponent
  {
    Ref<Mesh> mesh;

    MeshComponent(const Ref<Mesh>& mesh);
    MeshComponent(const MetaFileClass& metaClass);
  };
}
