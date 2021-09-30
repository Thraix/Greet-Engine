#include "TLBR.h"

namespace Greet
{
  TLBR::TLBR()
    : top{0}, left{0}, bottom{0},right{0}
  {}

  TLBR::TLBR(float topLeftBottomRight)
    : top{topLeftBottomRight}, left{topLeftBottomRight}, bottom{topLeftBottomRight}, right{topLeftBottomRight}
  {}

  TLBR::TLBR(float topBottom, float leftRight)
    : top{topBottom}, left{leftRight}, bottom{topBottom},right{leftRight}
  {}

  TLBR::TLBR(float top, float left, float bottom, float right)
    : top{top}, left{left}, bottom{bottom},right{right}
  {}

  Vec2f TLBR::LeftTop() const
  {
    return Vec2f(left, top);
  }

  Vec2f TLBR::RightBottom() const
  {
    return Vec2f(right, bottom);
  }

  float TLBR::GetWidth() const
  {
    return left + right;
  }

  float TLBR::GetHeight() const
  {
    return top + bottom;
  }

  Vec2f TLBR::GetSize() const
  {
    return Vec2f(GetWidth(), GetHeight());
  }
}
