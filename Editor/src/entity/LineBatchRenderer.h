#pragma once

#include <math/Mat4.h>
#include <math/Mat3.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Line.h>
#include <graphics/shaders/Shader.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/buffers/VertexBuffer.h>
#include <graphics/buffers/Buffer.h>
#include <vector>

namespace Greet
{
  class LineBatchRenderer final
  {
    private:
      struct LineVertexData
      {
        Vec3f vertex;
        uint32_t color;
        float is2D;
      };

      Ref<VertexBuffer> vbo;
      Ref<VertexArray> vao;
      Ref<Buffer> ibo;
      uint32_t iboSize = 0;
      std::vector<uint32_t> indices;
      LineVertexData* buffer;

      int lineWidth = 3;

      Ref<Shader> shader;
    public:
      LineBatchRenderer(int maxAmountOfLines = 10000);
      ~LineBatchRenderer();
      void SetPVMatrix(const Mat4& pvMatrix);
      void SetPVMatrix(const Mat3& pvMatrix);
      void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color);
      void DrawLine(const Vec3f& pos1, const Vec3f& pos2, const Color& color, bool is2D);
      void DrawLine(const Line& line, const Color& color);
      void DrawRectangle(const Mat3& transform, const Color& color);
      void DrawRectangle(const Vec2f& pos, const Vec2f& size, const Color& color);
      void DrawCuboid(const Mat4& transform, const Color& color);
      void DrawCuboid(const Vec3f& pos, const Vec3f& size, const Color& color);
      void Begin();
      void End();
      void SetLineWidth(int lineWidth);

    private:
      void Draw();

  };
}
