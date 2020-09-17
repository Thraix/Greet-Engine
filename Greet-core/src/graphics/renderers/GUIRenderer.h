#pragma once

#include <graphics/Color.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/fonts/Font.h>
#include <graphics/renderers/Renderer2D.h>
#include <math/Vec4.h>
#include <stack>

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
      uint m_bufferSize;
      uint m_iboSize;
      uint m_vertices;
      int maxTextures;


      Ref<Buffer> ibo;
      Ref<VertexBuffer> vbo;
      Ref<VertexArray> vao;
      uint m_lastIndex;
      uint* m_indices;
      uint m_iboCount;
      GUIVertex* m_buffer;
      GUIVertex* m_bufferBegin;

      uint* m_textures;
      byte m_textureCount;


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
      void DrawRoundedRect(const Vec2f& pos, const Vec2f& size, const Color& color, float radius, uint precision, bool isHsv = true);
      void DrawRect(const Vec2f& pos, const Vec2f& size, const Color& color1, const Color& color2, const Color& color3, const Color& color4, bool isHsv = true);

      void PushViewport(const Vec2f& pos, const Vec2f& size, bool overwrite=false);
      void PopViewport();

      void PushTranslation(const Vec2f& translation, bool override = false);
      void PopTranslation();
    private:
      float GetTextureSlot(uint Id);
      bool NeedFlush(uint indices, uint vertices);
      Vec4f GetViewport(const Vec2f& pos1, const Vec2f& pos2) const;
      void AppendTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color, bool isHsv);
      void AppendRoundedQuad(const Vec2f& position, const Vec2f& size, const Color& color, bool hsb, float radius, uint precision);
      void AppendQuaterCircle(const Vec2f& center, const Color& color, bool isHsv, float radius, uint precision, bool top, bool left);
      void AppendQuad(const Vec2f& position, const Vec2f& size, const Vec2f& texCoord1, const Vec2f& texCoord2, float texID, const Color& color1, const Color& color2, const Color& color3, const Color& color4, bool isHsv);
      void AppendQuad(const Vec2f& position, const Vec2f& size, const Vec2f& texCoord1, const Vec2f& texCoord2, float texID, const Color& color, bool isHsv);
      void AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, float texID, const Color& color, const Vec4f& viewport, bool isHsv);
  };
}
