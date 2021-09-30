#include "MeshComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  MeshComponent::MeshComponent(const Ref<Mesh>& mesh)
    : mesh{mesh}
  {}

  MeshComponent::MeshComponent(const MetaFileClass& metaClass)
    : mesh{MetaFileLoading::LoadMesh(metaClass, "mesh")}
  {}
}
