#pragma once

#include <common/Memory.h>
#include <graphics/Renderable2D.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/buffers/VertexBuffer.h>
#include <graphics/fonts/Font.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/textures/Texture2D.h>

#include <stdint.h>
#include <vector>

#define RENDERER_MAX_SPRITES	360000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX			0x00
#define SHADER_TEXCOORD_INDEX		0x01
#define SHADER_TEXID_INDEX			0x02
#define SHADER_COLOR_INDEX			0x03
#define SHADER_MASK_TEXCOORD_INDEX	0x04
#define SHADER_MASK_TEXID_INDEX		0x05

namespace Greet {

  class BatchRenderer : public Renderer2D
  {

    private:
      Ref<VertexBuffer> vbo;
      Ref<VertexArray> vao;
      Ref<Buffer> ibo;

      int maxTextures;
      uint32_t m_iboSize;
      uint32_t m_lastIndex;
      uint32_t* indices;
      VertexData* m_buffer;

      std::vector<uint32_t> m_texSlots;

    public:
      BatchRenderer(const Ref<Shader>& shader);
      virtual ~BatchRenderer();
      void Begin() override;

      void Draw(const Renderable2D& renderable) override;
      void DrawRect(const Mat3& transform, uint32_t color = 0xffffffff);
      void DrawRect(const Mat3& transform, const Ref<Texture2D>& texture, const Vec2f& texPos = Vec2f{0, 0}, const Vec2f& texSize = Vec2f{1, 1}, uint32_t color = 0xffffffff);

      void DrawRect(const Vec2f& position, const Vec2f& size, uint32_t color = 0xffffffff);
      void DrawRect(const Vec2f& position, const Vec2f& size, const Ref<Texture2D>& texture, const Vec2f& texPos = Vec2f{0, 0}, const Vec2f& texSize = Vec2f{1, 1}, uint32_t color = 0xffffffff);

      void DrawText(const std::string& text, const Vec2f& position, const Font& font, uint32_t color = 0xffffffff);
      void Draw(const Vec2f& position, const Vec2f* vertices, uint32_t amount, uint32_t color = 0xffffffff);
      void DrawLine(const Vec2f& pos1, const Vec2f& pos2, float width, uint32_t color = 0xffffffff);
      void AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, uint32_t texSlot, uint32_t color);
      uint32_t GetTextureSlot(const Ref<Texture2D>& texture);
      uint32_t GetTextureSlot(uint32_t texID);
      void End() override;
      void Flush() override;
      void EnableBuffers();
      void DisableBuffers();
      void AddIndicesPoly(uint32_t poly);
      void SetShader(const Ref<Shader>& aShader) override;
    private:
      void Init();
  };
}
