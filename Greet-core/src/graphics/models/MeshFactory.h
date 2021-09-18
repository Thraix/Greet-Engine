#pragma once

#include <common/Memory.h>
#include <graphics/models/MeshData.h>
#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet {
  struct MeshFactory
  {
    enum PolygonSizeFormat
    {
      SIDE_LENGTH, INSCRIBED_RADIUS, CIRCUMSCRIBED_RADIUS
    };

    static Vec3f CalculateNormal(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3);
    static void CalculateNormals(const Pointer<Vec3f>& vertices, const Pointer<uint32_t>& indices, Pointer<Vec3f>& normals);
    static Pointer<Vec3f> CalculateNormals(const Pointer<Vec3f> vertices, const Pointer<uint32_t>& indices);

    static MeshData Polygon(uint32_t count, PolygonSizeFormat format, const Vec3f& pos = {0, 0, 0}, float size = 1);

    static MeshData Plane(const Vec3f& pos = {0, 0, 0}, const Vec2f& size = {1, 1});
    static MeshData Cube(const Vec3f& pos = {0, 0, 0}, const Vec3f& size = {1, 1, 1});
    static MeshData Tetrahedron(const Vec3f& pos = {0, 0, 0}, float size = 1);
    static MeshData Grid(const Vec2i& gridSize, const Vec3f& pos = {0, 0, 0}, const Vec2f& size = {1, 1});
    static MeshData Grid(const Vec2i& gridSize, const Pointer<float>& heightMap, const Vec3f& pos = {0, 0, 0}, const Vec3f& size = {1, 1, 1});
    static MeshData GridLowPoly(const Vec2i& gridSize, const Pointer<float>& heightMap, const Vec3f& pos = {0, 0, 0}, const Vec3f& size = {1, 1, 1});
    static uint32_t IndexGrid(uint32_t x, uint32_t z, uint32_t gridWidth, uint32_t gridLength);
    static MeshData Sphere(uint32_t latitudes = 20, uint32_t longitudes = 20, const Vec3f& position = {0, 0, 0}, float radius = 1);
  };
}
