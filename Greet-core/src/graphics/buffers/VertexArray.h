#pragma once

#include <common/Memory.h>
#include <graphics/buffers/VertexBuffer.h>

#include <stdint.h>
#include <vector>

namespace Greet {

  enum class DrawType
  {
    TRIANGLES = 0x0004, LINES = 0x0001
  };

  class VertexArray final
  {
    private:
      uint32_t id;
      std::vector<Ref<VertexBuffer>> buffers;
      Ref<Buffer> indexBuffer;
    private:
      VertexArray();
    public:
      ~VertexArray();

      void Enable() const;
      void Disable();
      void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
      void SetIndexBuffer(const Ref<Buffer>& indexBuffer);

      void Render(DrawType type, uint32_t count);

    public:
      static Ref<VertexArray> Create();
  };

}
