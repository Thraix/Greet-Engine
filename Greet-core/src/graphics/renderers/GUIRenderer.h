#pragma once

#include <graphics/Color.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/fonts/Font.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/shaders/Shader.h>
#include <math/Vec2.h>

#include <stdint.h>

namespace Greet
{
  class GUI;

  class GUIRenderer : public Renderer2D
  {
    struct GUIVertex
    {
      Vec2f pos;
      Vec2f texCoord;
      float texId;
      Color color;
      Vec4f viewport;
      float isHSV;
    };

    private:
      std::stack<Vec4f> m_viewports;
      std::stack<Vec2f> translationStack;
      uint32_t m_bufferSize;
      uint32_t m_iboSize;
      uint32_t m_vertices;
      int maxTextures;


      Ref<Buffer> ibo;
      Ref<VertexBuffer> vbo;
      Ref<VertexArray> vao;
      uint32_t m_lastIndex;
      uint32_t* m_indices;
      uint32_t m_iboCount;
      GUIVertex* m_buffer;
      GUIVertex* m_bufferBegin;

      uint32_t* m_textures;
      uint8_t m_textureCount;


    public:
      GUIRenderer();
      GUIRenderer(const Ref<Shader>& shader);
      void Begin();
      void End();
      void Flush();
      void Draw();
      void Draw(const Renderable2D& renderable);
      void DrawLine(const Vec2f& pos, float length, float width, bool vertical, const Color& color, bool isHsv);
      void DrawText(const std::string& text, const Vec2f& position, const Font& font, const Color& color, bool isHsv = true);
      void DrawTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Color& color, bool isHsv = true);
      void DrawRoundedRect(const Vec2f& pos, const Vec2f& size, const Color& color, float radius, uint32_t precision, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Color& color1, const Color& color2, const Color& color3, const Color& color4, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Ref<Texture>& texture, const Color& color, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Ref<Texture>& texture, const Vec2f& texCoordPos, const Vec2f& texCoordSize, const Color& color, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Ref<Texture>& texture, const Vec2f& texCoordTL, const Vec2f& texCoordTR, const Vec2f& texCoordBL, const Vec2f& texCoordBR, const Color& color, bool isHsv = true);

      void PushViewport(const Vec2f& pos, const Vec2f& size, bool overwrite=false);
      void PopViewport();

      void PushTranslation(const Vec2f& translation, bool override = false);
      void PopTranslation();
    private:
      float GetTextureSlot(uint32_t Id);
      bool NeedFlush(uint32_t indices, uint32_t vertices);
      Vec4f GetViewport(const Vec2f& pos1, const Vec2f& pos2) const;
      void AppendTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color, bool isHsv);
      void AppendRoundedQuad(const Vec2f& position, const Vec2f& size, const Color& color, bool hsb, float radius, uint32_t precision);
      void AppendQuaterCircle(const Vec2f& center, const Color& color, bool isHsv, float radius, uint32_t precision, bool top, bool left);
      void AppendQuad(const Vec2f& position, const Vec2f& size, const Vec2f& texCoord1, const Vec2f& texCoord2, float texID, const Color& color1, const Color& color2, const Color& color3, const Color& color4, bool isHsv);
      void AppendQuad(const Vec2f& position, const Vec2f& size, const Vec2f& texCoord1, const Vec2f& texCoord2, float texID, const Color& color, bool isHsv);
      void AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, float texID, const Color& color, const Vec4f& viewport, bool isHsv);
  };
}
