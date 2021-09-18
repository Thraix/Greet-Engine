#pragma once

#include <graphics/buffers/Buffer.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/models/MeshData.h>

#include <map>

namespace Greet {

  class Mesh
  {
    private:
      // Location, vbo
      std::map<uint32_t, Ref<Buffer>> m_vbos;

      Ref<VertexArray> vao;
      Ref<Buffer> ibo;

      uint32_t m_vertexCount;
      uint32_t m_indexCount;
      DrawType m_drawMode;
      bool m_culling = true;
      bool m_clockwise = false;
      bool wireframe = false;

    public:
      Mesh(const Pointer<Vec3f>& vertices, const Pointer<uint32_t>& indices);
      Mesh(const MeshData& data);

      void Render() const;

      void Bind() const;
      void Unbind() const;

      inline void SetDrawMode(DrawType drawType) { m_drawMode = drawType; }
      inline void SetClockwiseRender(bool clockwise) { m_clockwise = clockwise; }
      inline bool IsClockwiseRender() const { return m_clockwise; }

      inline void SetEnableCulling(bool culling) { m_culling = culling; }
      inline bool IsEnableCulling() const { return m_culling; }

      inline void SetEnableWireframe(bool _wireframe) { wireframe = _wireframe; }
      inline bool IsEnableWireframe() const { return wireframe; }

      void AddAttribute(const BufferAttribute& attribute, const Pointer<char>& data);

      void SetDefaultAttribute4f(uint32_t location, const Vec4f& data);
      void SetDefaultAttribute3f(uint32_t location, const Vec3f& data);
    private:
      bool HasVBO(uint32_t location) const;
  };
}
