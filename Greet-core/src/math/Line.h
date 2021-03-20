#pragma once

#include <math/Vec3.h>

namespace Greet
{
  struct Line
  {
    Vec3f pos;
    Vec3f dir;

    Line()
      : pos{}, dir{0, 1, 0}
    {}

    Line(const Vec3f& pos, const Vec3f& dir)
      : pos{pos}, dir{dir}
    {}

    Line Points(const Vec3f& p1, const Vec3f& p2)
    {
      return Line{p1, p2 - p1};
    }

    Vec3f PointClosestFromLine(const Line& line)
    {
      // Math:
      // L1 = P1 + t * D1
      // L2 = P2 + s * D2
      // A = (P1.x + t * D1.x, P1.y + t * D1.y, P1.z + t * D1.z)
      // B = (P2.x + s * D2.x, P2.y + s * D2.y, P2.z + s * D2.z)
      // Dot(B - A, D1) = 0
      // Dot(B - A, D2) = 0
      // Solve for t

      // Might be ways to simplify this
      // Should also handle cases where we have division by 0 (when does this occur geometricly?)
      float d1d2 = Vec::Dot(dir, line.dir);
      float t = ((Vec::Dot(pos, dir) - Vec::Dot(dir, line.pos)) * Vec::Dot(line.dir, line.dir) / (d1d2 * d1d2) + (Vec::Dot(line.dir, line.pos) - Vec::Dot(line.dir, pos)) / d1d2) / (1 - Vec::Dot(dir, dir) * Vec::Dot(line.dir, line.dir) / (d1d2 * d1d2));
      return pos + dir * t;
    }
  };
}

