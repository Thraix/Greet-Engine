#pragma once

#include <math/Vec3.h>
#include <math/Line.h>
#include <math/Plane.h>
#include <common/Pointer.h>

namespace Greet
{
  struct BoundingBox
  {

    Vec3f boundingBoxMin;
    Vec3f boundingBoxMax;

    BoundingBox(const Vec3f& boundingBoxMin, const Vec3f& boundingBoxMax)
      : boundingBoxMin{boundingBoxMin}, boundingBoxMax{boundingBoxMax}
    {}

    BoundingBox(const Pointer<Vec3f>& vertices)
      : boundingBoxMin{0}, boundingBoxMax{0}
    {
      if(vertices.Size() > 0)
      {
        boundingBoxMin = vertices[0];
        boundingBoxMax = vertices[0];
        for(int i = 0;i < vertices.Size(); i++)
        {
          boundingBoxMin = Vec::Min(boundingBoxMin, vertices[i]);
          boundingBoxMax = Vec::Max(boundingBoxMax, vertices[i]);
        }
      }
    }

    std::pair<bool, float> LineIntersects(const Line& line, float distanceLimit) const
    {
      std::vector<Vec3f> points =
      {
        Plane{Vec3f{1, 0, 0}, Vec3f{boundingBoxMax.x, 0, 0}}.LineIntersection(line),
        Plane{Vec3f{0, 1, 0}, Vec3f{0, boundingBoxMax.y, 0}}.LineIntersection(line),
        Plane{Vec3f{0, 0, 1}, Vec3f{0, 0, boundingBoxMax.z}}.LineIntersection(line),
        Plane{Vec3f{-1, 0, 0}, Vec3f{boundingBoxMin.x, 0, 0}}.LineIntersection(line),
        Plane{Vec3f{0, -1, 0}, Vec3f{0, boundingBoxMin.y, 0}}.LineIntersection(line),
        Plane{Vec3f{0, 0, -1}, Vec3f{0, 0, boundingBoxMin.z}}.LineIntersection(line)
      };

      float distanceSq = distanceLimit * distanceLimit;
      bool collision = false;
      for(int i = 0; i < 6; i++)
      {
        int axis1 = (i + 1) % 3;
        int axis2 = (i + 2) % 3;
        if(points[i][axis1] > boundingBoxMin[axis1] &&
            points[i][axis1] < boundingBoxMax[axis1] &&
            points[i][axis2] > boundingBoxMin[axis2] &&
            points[i][axis2] < boundingBoxMax[axis2] &&
            (points[i] - line.pos).LengthSQ() < distanceSq)
        {
          distanceSq = (points[i] - line.pos).LengthSQ();
          collision = true;
        }
      }
      return {collision, sqrt(distanceSq)};
    }

    const Vec3f& GetPosition() const
    {
      return boundingBoxMin;
    }

    Vec3f GetSize() const
    {
      return boundingBoxMax - boundingBoxMin;
    }

    Vec3f GetCenter() const
    {
      return boundingBoxMin + (boundingBoxMax - boundingBoxMin) * 0.5f;
    }

    friend std::ostream& operator<<(std::ostream& stream, const BoundingBox& boundingBox)
    {
      return stream << boundingBox.boundingBoxMin << " - " << boundingBox.boundingBoxMax;
    }
  };
}
