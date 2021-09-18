#include "Chunk.h"

#include <graphics/models/MeshFactory.h>

using namespace Greet;

uint Chunk::CHUNK_WIDTH = 32;
uint Chunk::CHUNK_HEIGHT = 32;

Chunk::Chunk()
{}

void Chunk::Initialize(uint posX, uint posY)
{
  this->posX = posX;
  this->posY = posY;
  std::vector<float> noise = Noise::GenNoise(CHUNK_WIDTH+1, CHUNK_HEIGHT+1,4,16, 16,0.75f, posX * CHUNK_WIDTH, posY * CHUNK_HEIGHT);
  MeshData data = MeshFactory::GridLowPoly(Vec2i{CHUNK_WIDTH, CHUNK_HEIGHT}, noise, {0, 0, 0}, {CHUNK_WIDTH, 1.0f, CHUNK_HEIGHT});
  RecalcGrid(data);
  mesh = new Mesh(data);
}

Chunk::~Chunk()
{
  delete mesh;
}

void Chunk::RecalcPositions(Vec3f& vertex)
{
  float y = vertex.y;
  if (y < 0.45)
  {
    //y = 0.45f + Noise::PRNG(vertex->x, vertex->z)*0.01f;// + 0.03f*(rand() / (float)RAND_MAX - 0.5f);
  }
  else if (y < 0.48)
  {

  }
  else if (y < 0.58)
  {

  }
  else if (y < 0.65)
  {
    y = (pow(y - 0.58, 0.6) + 0.58);
  }
  else
  {
    y = (pow(y - 0.58, 0.6) + 0.58);
  }
  vertex.y = y * 20.0;
}

void RecalcColors(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, uint* color)
{
  float y = (v1.y + v2.y + v3.y) / 3.0f / 20.0f;
  if (y < 0.45+0.01f)
  {
    uint blue = (uint)(pow(1, 4.0f) * 255);
    blue = blue > 255 ? 255 : blue;
    *color = 0xff000000 | ((blue / 2) << 16) | ((uint)(blue * 0.9) << 8) | blue;
    *color = 0xffF0E5A5;
  }
  else if (y < 0.48)
  {
    *color = 0xffF0E5A5;
  }
  else if (y < 0.58)
  {
    *color = 0xff7CD663;
  }
  else if (y < pow(0.07, 0.6) + 0.58)
  {
    *color = 0xffB5B0A8;
  }
  else
  {
    *color = 0xffDCF2F2;
  }
}

void Chunk::CalcGridVertexOffset(MeshData* data)
{
  const Pointer<Vec3f>& vertices = data->GetVertices();
  const Pointer<uint>& indices = data->GetIndices();

  Pointer<uint8_t> offsets(4 * vertices.Size());
  for (int i = 0;i < indices.Size();i+=3)
  {
    Vec3f v1 = vertices[indices[i+1]] - vertices[indices[i]];
    Vec3f v2 = vertices[indices[i+2]] - vertices[indices[i]];
    offsets[indices[i]*4] = round(v1.x);
    offsets[indices[i]*4 + 1] = round(v1.z);
    offsets[indices[i]*4 + 2] = round(v2.x);
    offsets[indices[i]*4 + 3] = round(v2.z);
  }
  data->AddAttribute({4, BufferAttributeType::BYTE4}, offsets);
}

void Chunk::RecalcGrid(MeshData& data)
{
  Pointer<uint> colors(data.GetVertexCount());
  Pointer<Vec3f>& vertices = data.GetVertices();
  Pointer<uint>& indices = data.GetIndices();
  Pointer<Vec3f> normals = data.GetAttribute(MESH_NORMALS_LOCATION)->second;

  for (int i = 0;i < indices.Size();i+=3)
  {
    RecalcPositions(vertices[indices[i]]);
  }
  uint index = MeshFactory::IndexGrid(CHUNK_WIDTH, CHUNK_HEIGHT - 1, CHUNK_WIDTH, CHUNK_HEIGHT);
  RecalcPositions(vertices[MeshFactory::IndexGrid(CHUNK_WIDTH,CHUNK_HEIGHT-1,CHUNK_WIDTH,CHUNK_HEIGHT)]);
  index = MeshFactory::IndexGrid(0, CHUNK_HEIGHT, CHUNK_WIDTH, CHUNK_HEIGHT);
  RecalcPositions(vertices[index]);
  for (int i = 0;i < indices.Size();i += 3)
  {
    normals[indices[i]] = MeshFactory::CalculateNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    RecalcColors(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], &colors[indices[i]]);
  }
  data.AddAttribute({MESH_COLORS_LOCATION, BufferAttributeType::UBYTE4}, colors);
}
