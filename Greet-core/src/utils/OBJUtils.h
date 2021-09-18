#pragma once

#include <graphics/models/Mesh.h>
#include <math/Vec2.h>

#include <string>
#include <vector>

namespace Greet {
  class OBJUtils
  {
    private:
      static inline MeshData ErrorModel();
      static inline void ProcessVertex(const std::vector<std::string>& vertexData, std::vector<uint32_t>& indicesArray, const std::vector<Vec2f>& texCoords, const std::vector<Vec3f>& normals, Vec2f* texCoordsArray, Vec3f* normalsArray);
    public:
      static MeshData LoadObj(const std::string& filename);
  };
}
