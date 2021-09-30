#pragma once

#include <math/Vec2.h>

#include <iostream>

namespace Greet
{
  struct TLBR final
  {
    public:
      float top;
      float left;
      float bottom;
      float right;

      TLBR();
      TLBR(float topLeftBottomRight);
      TLBR(float topBottom, float leftRight);
      TLBR(float top, float left, float bottom, float right);

      Vec2f LeftTop() const;
      Vec2f RightBottom() const;
      float GetWidth() const;
      float GetHeight() const;
      Vec2f GetSize() const;

      friend std::ostream& operator<<(std::ostream& stream, const TLBR& tlbr)
      {
        return stream<< "(" << tlbr.top << ", " << tlbr.left << ", " << tlbr.bottom  << ", "<< tlbr.right << ")";
      }
  };
}
