#pragma once

#include <common/Pointer.h>
#include <graphics/buffers/VertexBuffer.h>
#include <math/Vec3.h>

#include <vector>

#define MESH_VERTICES_LOCATION	0
#define MESH_TEXCOORDS_LOCATION 1
#define MESH_COLORS_LOCATION	2
#define MESH_NORMALS_LOCATION	3

namespace Greet {

  class MeshData final
  {
    friend class Mesh;
    private:
      Pointer<Vec3f> m_vertices;
      Pointer<uint32_t> m_indices;
      std::vector<std::pair<BufferAttribute, Pointer<char>>> m_data;
    public:
      MeshData(const Pointer<Vec3f>& vertices, const Pointer<uint32_t>& indices);
      ~MeshData() {}

      void AddAttribute(const BufferAttribute& attribute, const Pointer<char>& data)
      {
        if(data.Size() / attribute.size != m_vertices.Size())
        {
          Log::Error("Attribute length doesn't match vertices length");
          Log::Error("Attribute Length: ", data.Size() / attribute.size);
          Log::Error("Vertices Length:  ", m_vertices.Size());
          ASSERT(false, "");
        }
        else
          m_data.push_back({attribute, data});
      }
      const std::pair<BufferAttribute, Pointer<char>>* GetAttribute(int location) const;
      std::pair<BufferAttribute, Pointer<char>>* RemoveAttribute(int location);

      const Pointer<Vec3f>& GetVertices() const { return m_vertices; }
      const Pointer<uint32_t>& GetIndices() const { return m_indices; }
      Pointer<Vec3f>& GetVertices() { return m_vertices; }
      Pointer<uint32_t>& GetIndices() { return m_indices; }
      uint32_t GetVertexCount() const { return m_vertices.Size(); }
      uint32_t GetIndexCount() const { return m_indices.Size(); }

      void GenerateNormals();
      MeshData* LowPolify();

      void WriteToFile(const std::string& filename) const;
      static MeshData ReadFromFile(const std::string& filename);
  };
}
