#pragma once

#include <math/Vec2.h>
#include <iostream>

// TODO: Move implementation to cpp file

namespace Greet
{
  struct TLBR
  {
    public:
      float top;
      float left;
      float bottom;
      float right;

      TLBR()
        : top{0}, left{0}, bottom{0},right{0}
      {}

      TLBR(float topLeftBottomRight)
        : top{topLeftBottomRight}, left{topLeftBottomRight}, bottom{topLeftBottomRight},right{topLeftBottomRight}
      {}

      TLBR(float topBottom, float leftRight)
        : top{topBottom}, left{leftRight}, bottom{topBottom},right{leftRight}
      {}

      TLBR(float top, float left, float bottom, float right)
        : top{top}, left{left}, bottom{bottom},right{right}
      {}

      friend std::ostream& operator<<(std::ostream& stream, const TLBR& tlbr)
      {
        return stream<< "(" << tlbr.top << ", " << tlbr.left << ", " << tlbr.bottom  << ", "<< tlbr.right << ")";
      }

      Vec2f LeftTop() const
      {
        return Vec2f(left, top);
      }

      Vec2f RightBottom() const
      {
        return Vec2f(right, bottom);
      }

      float GetWidth() const
      {
        return left + right;
      }

      float GetHeight() const
      {
        return top + bottom;
      }

      Vec2f GetSize() const
      {
        return Vec2f(GetWidth(), GetHeight());
      }
  };
}
