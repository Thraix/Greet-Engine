#include "LineBatchRenderer.h"

#include <numeric>
#include <graphics/RenderCommand.h>

// Might want to move this into Greet-core at some point

namespace Greet
{
  LineBatchRenderer::LineBatchRenderer(int maxAmountOfLines)
  {
    shader = Shader::FromFile("res/shaders/line.glsl");
    shader->Enable();

    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, maxAmountOfLines * 2 * sizeof(LineVertexData));
    vbo->SetStructure(
    {
      {shader->GetAttributeLocation("aPosition"), BufferAttributeType::VEC3},
      {shader->GetAttributeLocation("aColor"), BufferAttributeType::UBYTE4, true},
      {shader->GetAttributeLocation("aIs2D"), BufferAttributeType::FLOAT, false}
    });
    vao->AddVertexBuffer(vbo);
    vbo->Disable();

    indices.resize(maxAmountOfLines * 2);
    std::iota(indices.begin(), indices.end(), 0);
    ibo = Buffer::Create(maxAmountOfLines * 2 * sizeof(uint32_t), BufferType::INDEX, BufferDrawType::STATIC);
    ibo->UpdateData(indices.data());
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
    shader->Disable();
  }

  LineBatchRenderer::~LineBatchRenderer()
  {}

  void LineBatchRenderer::SetPVMatrix(const Mat3& pvMatrix)
  {
    shader->SetUniformMat3("uPVMatrix2D", pvMatrix);
  }

  void LineBatchRenderer::SetPVMatrix(const Mat4& pvMatrix)
  {
    shader->SetUniformMat4("uPVMatrix3D", pvMatrix);
  }

  void LineBatchRenderer::DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color)
  {
    DrawLine(Vec3f{pos1.x, pos1.y, 0.0f}, Vec3f{pos2.x, pos2.y, 0.0f}, color, true);
  }

  void LineBatchRenderer::DrawLine(const Vec3f& pos1, const Vec3f& pos2, const Color& color, bool is2D)
  {
    if(iboSize + 2 > indices.size())
    {
      End();
      Begin();
    }
    buffer->color = color.AsUInt();
    buffer->vertex = pos1;
    buffer->is2D = is2D ? 1.0f : 0.0f;
    ++buffer;
    buffer->color = color.AsUInt();
    buffer->vertex = pos2;
    buffer->is2D = is2D ? 1.0f : 0.0f;
    ++buffer;
    iboSize += 2;
  }

  void LineBatchRenderer::DrawLine(const Line& line, const Color& color)
  {
    DrawLine(line.pos, line.pos + line.dir, Color{1, 1, 1}, false);
  }

  void LineBatchRenderer::DrawRectangle(const Mat3& transform, const Color& color)
  {
    DrawLine(transform * Vec2f{ 0.5,  0.5}, transform * Vec2f{ 0.5, -0.5}, color);
    DrawLine(transform * Vec2f{ 0.5,  0.5}, transform * Vec2f{-0.5,  0.5}, color);
    DrawLine(transform * Vec2f{-0.5, -0.5}, transform * Vec2f{ 0.5, -0.5}, color);
    DrawLine(transform * Vec2f{-0.5, -0.5}, transform * Vec2f{-0.5,  0.5}, color);
  }

  void LineBatchRenderer::DrawRectangle(const Vec2f& pos, const Vec2f& size, const Color& color)
  {
    DrawLine(pos, pos + Vec2f{size.x, 0.0f}, color);
    DrawLine(pos, pos + Vec2f{0.0f, size.y}, color);
    DrawLine(pos + size, pos + Vec2f{size.x, 0.0f}, color);
    DrawLine(pos + size, pos + Vec2f{0.0f, size.y}, color);
  }

  void LineBatchRenderer::DrawCuboid(const Mat4& transform, const Color& color)
  {
    Log::Error("LineBatchRenderer::DrawCuboid not implemented");
  }

  void LineBatchRenderer::DrawCuboid(const Vec3f& pos, const Vec3f& size, const Color& color)
  {
    Log::Error("LineBatchRenderer::DrawCuboid not implemented");
  }

  void LineBatchRenderer::Begin()
  {
    shader->Enable();
    vbo->Enable();
    buffer = (LineVertexData*)vbo->MapBuffer();
    iboSize = 0;
  }

  void LineBatchRenderer::Draw()
  {
    RenderCommand::SetLineWidth(lineWidth);
    RenderCommand::EnableDepthTest(false);
    RenderCommand::EnableCulling(false);

    vao->Enable();
    vao->Render(DrawType::LINES, iboSize);
    vao->Disable();

    iboSize = 0;
    RenderCommand::ResetDepthTest();
    RenderCommand::ResetCulling();
    RenderCommand::ResetLineWidth();
  }

  void LineBatchRenderer::End()
  {
    vbo->UnmapBuffer();
    vbo->Disable();

    if(iboSize > 0)
      Draw();
    shader->Disable();
  }

  void LineBatchRenderer::SetLineWidth(int lineWidth)
  {
    this->lineWidth = lineWidth;
  }
}
