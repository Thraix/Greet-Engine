#pragma once

#include <string>
#include <math/Vec2.h>

#define BIT(x) (1 << (x))
#define BIND_MEMBER_FUNC(fn) [this](auto&&... args){ this->fn(std::forward<decltype(args)>(args)...); }
#define BIND_GLOBAL_FUNC(fn) &fn

namespace Greet {

  struct Utils
  {
    template<class T>
    struct ptr_less
    {
      bool operator()(const T* lhs, const T* rhs) const
      {
        return *lhs < *rhs;
      }
    };

    static const std::string CurrentDateTime();
    static void Screenshot(uint32_t width, uint32_t height, std::string filename = "");
    static std::string GetTime();
    static bool IsInside(const Vec2f& point, const Vec2f& pos, const Vec2f& size);
  };
}
