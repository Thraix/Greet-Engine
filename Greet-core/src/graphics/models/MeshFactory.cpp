#include "MeshFactory.h"

#include "graphics/models/MeshData.h"
#include <logging/Log.h>

namespace Greet {
  // Calculate a normal of 3 points in space
  Vec3f MeshFactory::CalculateNormal(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3)
  {
    Vec3f v1 = p2 - p1;
    Vec3f v2 = p3 - p1;
    Vec3f normal = v1.Cross(v2);
    return normal;
  }

  void MeshFactory::CalculateNormals(const Pointer<Vec3f>& vertices, const Pointer<uint32_t>& indices, Pointer<Vec3f>& normals)
  {
    Vec3f faceNormal;
    Vec3f p1, p2, p3;
    memset(normals.Data(), 0, sizeof(float) * 3 * vertices.Size());
    for (uint32_t i = 0; i < indices.Size(); i += 3)
    {
      if(indices[i] >= vertices.Size() ||
          indices[i+1] >= vertices.Size() ||
          indices[i+2] >= vertices.Size())
      {
        Log::Error("Index out of bound in CalculateNormals");
        continue;
      }
      p1 = vertices[indices[i]];
      p2 = vertices[indices[i + 1]];
      p3 = vertices[indices[i + 2]];
      faceNormal = CalculateNormal(p1, p2, p3);
      normals[indices[i]] += faceNormal;
      normals[indices[i + 1]] += faceNormal;
      normals[indices[i + 2]] += faceNormal;
    }
    for (uint32_t i = 0; i < vertices.Size(); i++)
    {
      normals[i].Normalize();
    }
  }

  // Calculate normals for a mesh
  Pointer<Vec3f> MeshFactory::CalculateNormals(const Pointer<Vec3f> vertices, const Pointer<uint32_t>& indices)
  {
    Pointer<Vec3f> normals(vertices.Size());
    CalculateNormals(vertices, indices, normals);
    return normals;
  }

  MeshData MeshFactory::Polygon(uint32_t count, PolygonSizeFormat format, const Vec3f& pos, float size)
  {
    if(count < 3)
    {
      Log::Warning("Invalid Polygon corners (returning square): ", count);
      count = 4;
    }
    uint32_t indexCount = 3 * (count - 2);
    Pointer<Vec3f> vertices = Pointer<Vec3f>(count);
    Pointer<Vec3f> normals(count);
    Pointer<uint32_t> indices = Pointer<uint32_t>(indexCount);
    float radius = 0;

    // 180 * (count - 2) / count
    float cornerAngle = M_PI * (1 - 2 / (float)count);

    switch(format)
    {
      case SIDE_LENGTH:
        radius = size / (2 * cos(cornerAngle/2));
        break;
      case CIRCUMSCRIBED_RADIUS:
        radius = size;
        break;
      case INSCRIBED_RADIUS:
        radius = size / sin(cornerAngle/2);
        break;
    }
    float angle = 2 * M_PI / (float)count;
    for(uint32_t i = 0; i < count;i++)
    {
      vertices[i] = pos + Vec3f(cos(angle/2 + angle*i), 0, sin(angle/2 + angle*i)) * radius;
      normals[i] = Vec3f(0,1,0);
    }

    for(uint32_t i = 0;i < count - 2; i++)
    {
      indices[i*3 + 0] = 0;
      indices[i*3 + 1] = 2 + i;
      indices[i*3 + 2] = 1 + i;
    }

    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    return data;
  }

  MeshData MeshFactory::Plane(const Vec3f& pos, const Vec2f& size)
  {
    Pointer<Vec3f> vertices = Pointer<Vec3f>(4);
    float halfWidth = size.w / 2.0f;
    float halfLength = size.h / 2.0f;
    vertices[0]  = Vec3f(pos.x-halfWidth, pos.y, pos.z-halfLength);
    vertices[1]  = Vec3f(pos.x+halfWidth, pos.y, pos.z-halfLength);
    vertices[2]  = Vec3f(pos.x+halfWidth, pos.y, pos.z+halfLength);
    vertices[3]  = Vec3f(pos.x-halfWidth, pos.y, pos.z+halfLength);

    Pointer<Vec3f> normals = Pointer<Vec3f>(4);
    normals[0]  = Vec3f(0.0f, 1.0f, 0.0f);
    normals[1]  = Vec3f(0.0f, 1.0f, 0.0f);
    normals[2]  = Vec3f(0.0f, 1.0f, 0.0f);
    normals[3]  = Vec3f(0.0f, 1.0f, 0.0f);

    Pointer<Vec2f> texCoords = Pointer<Vec2f>(4);
    texCoords[0] = Vec2f(0.0f, 0.0f);
    texCoords[1] = Vec2f(1.0f, 0.0f);
    texCoords[2] = Vec2f(1.0f, 1.0f);
    texCoords[3] = Vec2f(0.0f, 1.0f);

    Pointer<uint32_t> indices{0,2,1,0,3,2};
    MeshData meshdata(vertices,indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    meshdata.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, texCoords);
    return meshdata;
  }
  // x, y, z
  MeshData MeshFactory::Cube(const Vec3f& pos, const Vec3f& size)
  {
    Pointer<Vec3f> vertices = Pointer<Vec3f>(24);
    float halfWidth = size.x / 2.0f;
    float halfHeight = size.y / 2.0f;
    float halfLength = size.z / 2.0f;


    vertices[0] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z - halfLength);
    vertices[1] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z - halfLength);
    vertices[2] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z + halfLength);
    vertices[3] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z + halfLength);

    vertices[4] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z - halfLength);
    vertices[5] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z - halfLength);
    vertices[6] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z + halfLength);
    vertices[7] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z + halfLength);

    vertices[8] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z - halfLength);
    vertices[9] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z + halfLength);
    vertices[10] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z + halfLength);
    vertices[11] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z - halfLength);

    vertices[12] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z - halfLength);
    vertices[13] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z + halfLength);
    vertices[14] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z + halfLength);
    vertices[15] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z - halfLength);


    vertices[16] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z - halfLength);
    vertices[17] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z - halfLength);
    vertices[18] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z - halfLength);
    vertices[19] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z - halfLength);

    vertices[20] = Vec3f(pos.x - halfWidth, pos.y - halfHeight, pos.z + halfLength);
    vertices[21] = Vec3f(pos.x + halfWidth, pos.y - halfHeight, pos.z + halfLength);
    vertices[22] = Vec3f(pos.x + halfWidth, pos.y + halfHeight, pos.z + halfLength);
    vertices[23] = Vec3f(pos.x - halfWidth, pos.y + halfHeight, pos.z + halfLength);

    Pointer<Vec3f> normals = Pointer<Vec3f>(24);
    normals[0] = Vec3f(-1.0f, 0.0f, 0.0f);
    normals[1] = Vec3f(-1.0f, 0.0f, 0.0f);
    normals[2] = Vec3f(-1.0f, 0.0f, 0.0f);
    normals[3] = Vec3f(-1.0f, 0.0f, 0.0f);

    normals[4] = Vec3f(1.0f, 0.0f, 0.0f);
    normals[5] = Vec3f(1.0f, 0.0f, 0.0f);
    normals[6] = Vec3f(1.0f, 0.0f, 0.0f);
    normals[7] = Vec3f(1.0f, 0.0f, 0.0f);

    normals[8] = Vec3f(0.0f, -1.0f, 0.0f);
    normals[9] = Vec3f(0.0f, -1.0f, 0.0f);
    normals[10] = Vec3f(0.0f, -1.0f, 0.0f);
    normals[11] = Vec3f(0.0f, -1.0f, 0.0f);

    normals[12] = Vec3f(0.0f, 1.0f, 0.0f);
    normals[13] = Vec3f(0.0f, 1.0f, 0.0f);
    normals[14] = Vec3f(0.0f, 1.0f, 0.0f);
    normals[15] = Vec3f(0.0f, 1.0f, 0.0f);

    normals[16] = Vec3f(0.0f, 0.0f, -1.0f);
    normals[17] = Vec3f(0.0f, 0.0f, -1.0f);
    normals[18] = Vec3f(0.0f, 0.0f, -1.0f);
    normals[19] = Vec3f(0.0f, 0.0f, -1.0f);

    normals[20] = Vec3f(0.0f, 0.0f, 1.0f);
    normals[21] = Vec3f(0.0f, 0.0f, 1.0f);
    normals[22] = Vec3f(0.0f, 0.0f, 1.0f);
    normals[23] = Vec3f(0.0f, 0.0f, 1.0f);


    float epsilon = 0.0001;
    Pointer<Vec2f> texCoords = Pointer<Vec2f>(24);
    texCoords[0] = Vec2f(0.00f, 1.0f / 3.0f + epsilon);
    texCoords[1] = Vec2f(0.00f, 2.0f / 3.0f - epsilon);
    texCoords[2] = Vec2f(0.25f, 2.0f / 3.0f - epsilon);
    texCoords[3] = Vec2f(0.25f, 1.0f / 3.0f + epsilon);

    texCoords[4] = Vec2f(0.75f, 1.0f / 3.0f + epsilon);
    texCoords[5] = Vec2f(0.75f, 2.0f / 3.0f - epsilon);
    texCoords[6] = Vec2f(0.50f, 2.0f / 3.0f - epsilon);
    texCoords[7] = Vec2f(0.50f, 1.0f / 3.0f + epsilon);

    texCoords[8]  = Vec2f(0.25f, 1.0f / 3.0f - epsilon);
    texCoords[9]  = Vec2f(0.25f, 0.00f + epsilon);
    texCoords[10] = Vec2f(0.50f, 0.00f + epsilon);
    texCoords[11] = Vec2f(0.50f, 1.0f / 3.0f - epsilon);

    texCoords[12] = Vec2f(0.25f, 1.00f - epsilon);
    texCoords[13] = Vec2f(0.25f, 2.0f / 3.0f + epsilon);
    texCoords[14] = Vec2f(0.50f, 2.0f / 3.0f + epsilon);
    texCoords[15] = Vec2f(0.50f, 1.00f - epsilon);

    texCoords[16] = Vec2f(1.00f, 1.0f / 3.0f + epsilon);
    texCoords[17] = Vec2f(0.75f, 1.0f / 3.0f + epsilon);
    texCoords[18] = Vec2f(0.75f, 2.0f / 3.0f - epsilon);
    texCoords[19] = Vec2f(1.00f, 2.0f / 3.0f - epsilon);

    texCoords[20] = Vec2f(0.25f, 1.0f / 3.0f + epsilon);
    texCoords[21] = Vec2f(0.50f, 1.0f / 3.0f + epsilon);
    texCoords[22] = Vec2f(0.50f, 2.0f / 3.0f - epsilon);
    texCoords[23] = Vec2f(0.25f, 2.0f / 3.0f - epsilon);


    Pointer<uint32_t> indices{
      0, 2, 1, 0, 3, 2,
        4, 5, 6, 4, 6, 7,
        8, 10, 9, 8, 11, 10,
        12, 13, 14, 12, 14, 15,
        16, 18, 17, 16, 19, 18,
        20, 21, 22, 20, 22, 23 };

    MeshData meshdata(vertices, indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    meshdata.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, texCoords);
    return meshdata;
  }

  MeshData MeshFactory::Tetrahedron(const Vec3f& pos, float size)
  {
    float tan30 = 0.5773502f;  //   tan(30/180*PI)
    float cos30 = 1.1547005f; //   1 / cos(30/180*PI)
    float halfSize = size * 0.5f;
    float l1 = halfSize*cos30;
    float l2 = halfSize*tan30; //
    float height = size*0.8164965f;// Height of tetrahedron
    float corner = size*0.4082483f; // Size/sqrt(6) -> Center to corner
    float face = height - corner; // Center to face

    Vec3f v1 = Vec3f(pos.x, pos.y-face, pos.z + l1);
    Vec3f v2 = Vec3f(pos.x + halfSize, pos.y - face, pos.z - l2);
    Vec3f v3 = Vec3f(pos.x - halfSize, pos.y - face, pos.z - l2);
    Vec3f v4 = Vec3f(pos.x, pos.y + corner, pos.z);


    Pointer<Vec3f> vertices = Pointer<Vec3f>(4);
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    vertices[3] = v4;


    Pointer<Vec3f> normals = Pointer<Vec3f>(4);

    normals[0] = (v1 - pos).Normalize();
    normals[1] = (v2 - pos).Normalize();
    normals[2] = (v3 - pos).Normalize();
    normals[3] = (v4 - pos).Normalize();

    Pointer<uint32_t> indices{ 0, 2, 1, 1, 3, 0, 2, 0, 3, 1, 2, 3};
    MeshData meshdata(vertices, indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);
    return meshdata;
  }

  MeshData MeshFactory::Grid(const Vec2i& gridSize, const Vec3f& pos, const Vec2f& size)
  {
    return Grid(gridSize, {}, pos, {size.x, 0, size.y});
  }

  MeshData MeshFactory::Grid(const Vec2i& gridSize, const Pointer<float>& heightMap, const Vec3f& pos, const Vec3f& size)
  {
    if (gridSize.w < 1 || gridSize.h < 1)
      return Plane(pos, Vec2f{size.x, size.z});

    float tileWidth = size.x / (float)gridSize.x;
    float tileLength = size.z / (float)gridSize.y;

    uint32_t vertexCount = (gridSize.x + 1) * (gridSize.y + 1);

    Pointer<Vec3f> vertices(vertexCount);

    // decenter x and y
    float x = pos.x - size.x / 2.0f;
    float z = pos.z - size.z / 2.0f;


    for (uint32_t iz = 0; iz <= gridSize.y; iz++)
    {
      for (uint32_t ix = 0; ix <= gridSize.x; ix++)
      {
        float heightM = heightMap.Size() == 0 ? 0 : heightMap[ix + iz*(gridSize.x + 1)];
        vertices[ix + iz*(gridSize.x + 1)] = Vec3f(x + ix*tileWidth, pos.y + heightM * size.y, z + iz*tileLength);
      }
    }

    uint32_t indexCount = 6 * gridSize.x * gridSize.y;
    Pointer<uint32_t> indices(indexCount);

    // Loop through every square
    uint32_t i = 0;
    for (uint32_t iz = 0; iz < gridSize.y; iz++)
    {
      for (uint32_t ix = 0; ix < gridSize.x; ix++)
      {
        if (ix % 2 == iz % 2)
        {
          indices[i++] = ix + iz * (gridSize.x + 1);
          indices[i++] = ix + (iz + 1) * (gridSize.x + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridSize.x + 1);
          indices[i++] = ix + 1 + iz * (gridSize.x + 1);
          indices[i++] = ix + iz * (gridSize.x + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridSize.x + 1);
        }
        else
        {
          indices[i++] = ix + iz * (gridSize.x + 1);
          indices[i++] = ix + (iz + 1) * (gridSize.x + 1);
          indices[i++] = ix + 1 + iz * (gridSize.x + 1);
          indices[i++] = ix + 1 + iz * (gridSize.x + 1);
          indices[i++] = ix + (iz + 1) * (gridSize.x + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridSize.x + 1);
        }
      }
    }

    Pointer<Vec3f> normals = CalculateNormals(vertices, indices);
    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);

    return data;
  }

  uint32_t MeshFactory::IndexGrid(uint32_t x, uint32_t z, uint32_t gridWidth, uint32_t gridLength)
  {
    if (z >= gridLength - 1)
    {
      return x + (z - (gridLength - 1)) * (gridWidth+1) + (gridLength - 1) * gridWidth * 2;
    }
    else if (x == 0)
    {
      return z * 2 * gridWidth;
    }
    else if (x == gridWidth)
    {
      return x * 2 - 1 + z * 2 * gridWidth;
    }
    else
    {
      return x * 2 - 1 + z * 2 * gridWidth;
    }
    return 0;
  }

  MeshData MeshFactory::GridLowPoly(const Vec2i& gridSize, const Pointer<float>& heightMap, const Vec3f& pos, const Vec3f& size)
  {
    if (gridSize.x < 1 || gridSize.y < 1)
      return Plane(pos, Vec2f{size.x, size.z});

    float tileWidth = size.x / (float)gridSize.x;
    float tileLength = size.z / (float)gridSize.y;

    uint32_t vertexCount = (gridSize.x + 1) * (gridSize.y + 1) + (gridSize.x - 1)*(gridSize.y - 1);

    Pointer<Vec3f> vertices = Pointer<Vec3f>(vertexCount);

    // decenter x and y
    float x = pos.x - size.x / 2.0f;
    float z = pos.z - size.z / 2.0f;

    for (uint32_t iz = 0; iz <= gridSize.y; iz++)
    {
      for (uint32_t ix = 0; ix <= gridSize.x; ix++)
      {
        float heightM = heightMap.Size() == 0 ? 0 : heightMap[ix + iz * (gridSize.x + 1)];
        Vec3f vec = Vec3f(x + ix*tileWidth, pos.y + heightM * size.y, z + iz*tileLength);
        uint32_t index = IndexGrid(ix,iz,gridSize.x, gridSize.y);
        vertices[index] = vec;
        if(iz < gridSize.y - 1 && ix != 0 && ix != gridSize.x)
        {
          vertices[index + 1] = vec;
        }
      }
    }

    uint32_t indexCount = 6 * gridSize.x * gridSize.y;
    Pointer<uint32_t> indices = Pointer<uint32_t>(indexCount);

    // Loop through every square
    uint32_t i = 0;
    for (uint32_t iz = 0; iz < gridSize.y; iz++)
    {
      for (uint32_t ix = 0; ix < gridSize.x; ix++)
      {
        if (iz < gridSize.y - 1)
        {
          uint32_t add = (ix != 0) ? 1 : 0;
          if (ix % 2 == iz % 2)
          {
            indices[i++] = IndexGrid(ix, iz,gridSize.x,gridSize.y) + add;
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
          }
          else
          {
            indices[i++] = IndexGrid(ix, iz, gridSize.x, gridSize.y) + add;
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
          }
        }
        else
        {
          if (ix % 2 == iz % 2)
          {
            indices[i++] = IndexGrid(ix, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz, gridSize.x, gridSize.y);
          }
          else
          {
            indices[i++] = IndexGrid(ix, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix + 1, iz, gridSize.x, gridSize.y);
            indices[i++] = IndexGrid(ix, iz + 1, gridSize.x, gridSize.y);
          }
        }

      }
    }

    Pointer<Vec3f> normals = CalculateNormals(vertices,indices);
    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);

    return data;
  }

  MeshData MeshFactory::Sphere(uint32_t latitudes, uint32_t longitudes, const Vec3f& pos, float radius)
  {
    if(latitudes < 1 || longitudes < 4)
    {
      Log::Error("Creating sphere failed. Latitude or longitude is too small.");
      Log::Error("Has to be atleast 1 and 4 respectivly");
      return Cube(pos, {radius, radius, radius});
    }

    uint32_t vertexCount = (longitudes + 1) * (latitudes + 2);
    Pointer<Vec3f> vertices = Pointer<Vec3f>(vertexCount);
    Pointer<Vec2f> texCoords = Pointer<Vec2f>(vertexCount);

    float anglePitch = M_PI / (latitudes + 1);
    float angleYaw = 2 * M_PI / longitudes;
    uint32_t i = 0;
    for(uint32_t lon = 0; lon <= longitudes; ++lon)
    {
      float angleYaw = 2 * M_PI * lon / longitudes;
      vertices[i] = pos + Vec3f{0, radius, 0};
      texCoords[i++] = Vec2f{-angleYaw / (float)M_PI / 2.0f, 1};
      for(uint32_t lat = 0; lat < latitudes; ++lat)
      {
        float anglePitch = (lat + 1) / (float)(latitudes + 2) * M_PI;
        float y = pos.y + radius * cos(anglePitch);
        float r = sqrt(radius*radius - y * y);
        float x = r * cos(angleYaw);
        float z = r * sin(angleYaw);
        vertices[i] = pos + Vec3f{x, y, z};
        texCoords[i++] = Vec2f{-angleYaw / (float)M_PI / 2.0f, (cos(anglePitch) + 1) * 0.5f};
      }
      vertices[i] = pos + Vec3f{0, -radius, 0};
      texCoords[i++] = Vec2f{-angleYaw / (float)M_PI / 2.0f, 0};
    }

    uint32_t indexCount = 2 * 3 * (longitudes) + 2 * 3 * longitudes * (latitudes - 1);

    Pointer<uint32_t> indices = Pointer<uint32_t>(indexCount);
    uint32_t index = 0;

    for(int lon = 0; lon < longitudes; lon++)
    {
      indices[index++] = lon * (latitudes + 2);
      indices[index++] = (lon + 1) * (latitudes + 2) + 1;
      indices[index++] = lon * (latitudes + 2) + 1;
    }

    for(int lon = 0; lon < longitudes; ++lon)
    {
      for(int lat = 0; lat < latitudes - 1; ++lat)
      {
        indices[index++] = lon * (latitudes + 2) + 1 + lat;
        indices[index++] = (lon + 1) * (latitudes + 2) + 1 + lat;
        indices[index++] = lon * (latitudes + 2) + 1 + lat + 1;

        indices[index++] = lon * (latitudes + 2) + 1 + lat + 1;
        indices[index++] = (lon + 1) * (latitudes + 2) + 1 + lat;
        indices[index++] = (lon + 1) * (latitudes + 2) + 1 + lat + 1;
      }
    }

    for(int lon = 0; lon < longitudes; lon++)
    {
      indices[index++] = (lon) * (latitudes + 2) + latitudes;
      indices[index++] = (lon + 1) * (latitudes + 2) + latitudes;
      indices[index++] = lon * (latitudes + 2) + 1 + latitudes;
    }

    MeshData data{vertices, indices};
    Pointer<Vec3f> normals = CalculateNormals(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    data.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, texCoords);

    return data;
  }

}
