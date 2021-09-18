#include "BatchRenderer.h"

#include <graphics/RenderCommand.h>
#include <internal/GreetGL.h>

#include <numeric>

namespace Greet {

  BatchRenderer::BatchRenderer(const Ref<Shader>& shader)
    : Renderer2D{shader}
  {
    // TODO: Make this into a RenderCommand.
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

    std::vector<int> samplerUnits(maxTextures);
    std::iota(samplerUnits.begin(), samplerUnits.end(), 0);
    shader->Enable();
    shader->SetUniform1iv("uTextures", maxTextures, samplerUnits.data());
    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, RENDERER_BUFFER_SIZE);
    vbo->SetStructure({
        { shader->GetAttributeLocation("aPosition"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("aTexCoord"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("aTexID"), BufferAttributeType::FLOAT},
        { shader->GetAttributeLocation("aColor"), BufferAttributeType::UBYTE4, true}
        });
    vao->AddVertexBuffer(vbo);
    vbo->Disable();

    indices = new uint32_t[RENDERER_INDICES_SIZE];
    ibo = Buffer::Create(sizeof(indices), BufferType::INDEX, BufferDrawType::DYNAMIC);
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
    shader->Disable();
  }

  BatchRenderer::~BatchRenderer()
  {
    delete[] indices;
  }

  void BatchRenderer::Begin()
  {
    shader->Enable();
    vbo->Enable();
    m_buffer = (VertexData*)vbo->MapBuffer();
    m_iboSize = 0;
    m_lastIndex = 0;
  }

  void BatchRenderer::Draw(const Renderable2D& renderable)
  {
    DrawRect(renderable.GetPosition(), renderable.GetSize(), renderable.GetTexture(), renderable.GetTexPos(), renderable.GetTexSize(), renderable.GetColor());
  }

  void BatchRenderer::DrawText(const std::string& text, const Vec2f& position, const Font& font, uint32_t color)
  {
    float ts = GetTextureSlot(font.GetFontAtlasId());
    if (ts == 0 && font.GetFontAtlasId() != 0)
    {
      End();
      Flush();
      Begin();
      ts = GetTextureSlot(font.GetFontAtlasId());
    }

    const Ref<FontAtlas>& atlas = font.GetFontAtlas();
    Vec2f pos;
    Vec2f size;
    Vec2f uv0;
    Vec2f uv1;
    Vec2f roundPos = Vec2f(round(position.x), round(position.y));
    float x = roundPos.x;
    for (uint32_t i = 0;i < text.length();i++)
    {
      const Glyph& glyph = atlas->GetGlyph(text[i]);
      pos.x = x + glyph.miBearingX;
      pos.y = (roundPos.y - glyph.miBearingY);
      size.x = glyph.miWidth;
      size.y = glyph.miHeight;

      uv0.x = glyph.mvTextureCoords.left;
      uv0.y = 1.0-glyph.mvTextureCoords.top;
      uv1.x = glyph.mvTextureCoords.right;
      uv1.y = 1.0-glyph.mvTextureCoords.bottom;

      AppendVertexBuffer(Vec2f(pos.x       , pos.y       ), Vec2f(uv0.x,uv0.y), ts, color);
      AppendVertexBuffer(Vec2f(pos.x       , pos.y+size.y), Vec2f(uv0.x,uv1.y), ts, color);
      AppendVertexBuffer(Vec2f(pos.x+size.x, pos.y+size.y), Vec2f(uv1.x,uv1.y), ts, color);
      AppendVertexBuffer(Vec2f(pos.x+size.x, pos.y       ), Vec2f(uv1.x,uv0.y), ts, color);

      AddIndicesPoly(4);
      m_iboSize += 6;

      x += glyph.miAdvanceX;
    }
  }

  void BatchRenderer::DrawRect(const Mat3& transform, uint32_t color)
  {
    DrawRect(transform, nullptr, {0, 0}, {1,1}, color);
  }

  void BatchRenderer::DrawRect(const Mat3& transform, const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize, uint32_t color)
  {
    PushMatrix(transform);

    float slot = GetTextureSlot(texture);

    AppendVertexBuffer(Vec2f(-0.5f, -0.5f), Vec2f(texPos.x, texPos.y), slot, color);
    AppendVertexBuffer(Vec2f(-0.5f, 0.5f), Vec2f(texPos.x, texPos.y + texSize.y), slot, color);
    AppendVertexBuffer(Vec2f(0.5f, 0.5f), Vec2f(texPos.x + texSize.x, texPos.y + texSize.y), slot, color);
    AppendVertexBuffer(Vec2f(0.5f, -0.5f), Vec2f(texPos.x + texSize.x, texPos.y), slot, color);

    PopMatrix();
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::DrawRect(const Vec2f& position, const Vec2f& size, const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize, uint32_t color)
  {
    uint32_t ts = GetTextureSlot(texture);

    AppendVertexBuffer(Vec2f(position.x, position.y), Vec2f(texPos.x, texPos.y), ts, color);
    AppendVertexBuffer(Vec2f(position.x, position.y + size.y), Vec2f(texPos.x, texPos.y + texSize.y), ts, color);
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y + size.y), Vec2f(texPos.x + texSize.x, texPos.y + texSize.y), ts, color);
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(texPos.x + texSize.x, texPos.y), ts, color);
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Vec2f& position, const Vec2f* vertices, uint32_t amount, uint32_t color)
  {
    for (uint32_t i = 0; i < amount; i++)
    {
      AppendVertexBuffer(position+vertices[i],Vec2f(0,0), 0, color);
    }
    AddIndicesPoly(amount);
    m_iboSize += (amount-2)*3;
  }

  void BatchRenderer::DrawRect(const Vec2f& position, const Vec2f& size, uint32_t color)
  {
    DrawRect(position, size, nullptr, {0, 0}, {1,1}, color);
  }

  void BatchRenderer::DrawLine(const Vec2f& pos1, const Vec2f& pos2, float width, uint32_t color)
  {
    Vec2f delta = pos2-pos1;
    float alpha = atan(delta.y / delta.x);
    Vec2f p = Vec2f{sin(alpha), cos(alpha)} * width * 0.5f;

    // This if is needed to avoid culling problems
    if(delta.x >= 0)
    {
      AppendVertexBuffer(pos1+Vec2f{p.x, -p.y}, Vec2f(0, 0), 0, color);
      AppendVertexBuffer(pos1+Vec2f{-p.x, p.y}, Vec2f(0, 1), 0, color);
      AppendVertexBuffer(pos2+Vec2f{-p.x, p.y}, Vec2f(1, 1), 0, color);
      AppendVertexBuffer(pos2+Vec2f{p.x, -p.y}, Vec2f(1, 0), 0, color);
    }
    else
    {
      AppendVertexBuffer(pos2+Vec2f{p.x, -p.y}, Vec2f(0, 0), 0, color);
      AppendVertexBuffer(pos2+Vec2f{-p.x, p.y}, Vec2f(0, 1), 0, color);
      AppendVertexBuffer(pos1+Vec2f{-p.x, p.y}, Vec2f(1, 1), 0, color);
      AppendVertexBuffer(pos1+Vec2f{p.x, -p.y}, Vec2f(1, 0), 0, color);
    }

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, uint32_t texSlot, uint32_t color)
  {
    m_buffer->vertex = *m_transformationBack*position;
    m_buffer->texCoord = texCoord;
    m_buffer->texID = texSlot;
    m_buffer->color = color;
    m_buffer++;
  }

  uint32_t BatchRenderer::GetTextureSlot(const Ref<Texture2D>& texture)
  {
    return GetTextureSlot(texture ? texture->GetTexId() : 0);
  }

  uint32_t BatchRenderer::GetTextureSlot(uint32_t texID)
  {
    if (texID == 0)
      return 0.0f;
    uint32_t ts = 0.0f;
    bool found = false;
    const int size = m_texSlots.size();
    for (int i = 0; i < size; i++)
    {
      if (m_texSlots[i] == texID)
      {
        ts = (uint32_t)(i + 1);
        found = true;
        break;
      }
    }
    if (!found)
    {
      if (size >= maxTextures)
      {
        End();
        Flush();
        Begin();
      }
      m_texSlots.push_back(texID);
      ts = (uint32_t)(size + 1);
    }
    return ts;
  }

  void BatchRenderer::End()
  {
    vbo->UnmapBuffer();
    vbo->Disable();
  }

  void BatchRenderer::Flush()
  {
    RenderCommand::EnableDepthTest(false);
    RenderCommand::EnableCulling(false);
    for (uint32_t i = 0; i < m_texSlots.size(); i++)
    {
      GLCall(glActiveTexture(GL_TEXTURE0 + i));
      GLCall(glBindTexture(GL_TEXTURE_2D, m_texSlots[i]));
    }

    EnableBuffers();
    vao->Render(DrawType::TRIANGLES, m_iboSize);
    DisableBuffers();

    m_iboSize = 0;
    m_texSlots.clear();
    GLCall(glActiveTexture(GL_TEXTURE0));
    RenderCommand::ResetDepthTest();
    RenderCommand::ResetCulling();
    shader->Disable();
  }

  void BatchRenderer::EnableBuffers()
  {
    vao->Enable();
    ibo->UpdateData(indices, m_iboSize*sizeof(uint32_t));
  }

  void BatchRenderer::DisableBuffers()
  {
    vao->Disable();
  }

  void BatchRenderer::AddIndicesPoly(uint32_t vertices)
  {
    vertices -= 2;
    for (uint32_t i = 0; i < vertices; i++)
    {
      indices[m_iboSize + 3 * i] = m_lastIndex;
      indices[m_iboSize + 3 * i + 1] = m_lastIndex + i+1;
      indices[m_iboSize + 3 * i + 2] = m_lastIndex + i+2;

    }
    m_lastIndex = indices[m_iboSize + 3 * (vertices - 1) + 2] + 1;
  }

  void BatchRenderer::SetShader(const Ref<Shader>& aShader)
  {
    Renderer2D::SetShader(aShader);
    std::vector<int> samplerUnits(maxTextures);
    std::iota(samplerUnits.begin(), samplerUnits.end(), 0);
    shader->Enable();
    shader->SetUniform1iv("uTextures", maxTextures, samplerUnits.data());
  }
}
