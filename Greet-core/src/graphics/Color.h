#pragma once

#include <math/Vec3.h>
#include <math/Vec4.h>

#include <ostream>

// TODO: Implement this in a cpp file
namespace Greet
{
  struct Color
  {
    union
    {
      float vals[4];
      struct
      {
        float r;
        float g;
        float b;
        float a;
      };
      struct
      {
        float h;
        float s;
        float v;
        float _a;
      };
    };

    Color()
      : r{0}, g{0}, b{0}, a{0}
    {}

    Color(float r, float g, float b)
      : r{r}, g{g}, b{b}, a{1}
    {}

    Color(float r, float g, float b, float a)
      : r{r}, g{g}, b{b}, a{a}
    {}

    Color(const Vec3f& color)
      : r{color.x}, g{color.y}, b{color.z}, a{1}
    {}

    Color(const Vec4f& color)
      : r{color.x}, g{color.y}, b{color.z}, a{color.w}
    {}

    Color(uint32_t argb32)
      : r{((argb32 >> 16) & 0xff) / 255.0f}, g{((argb32 >> 8) & 0xff) / 255.0f}, b{(argb32 & 0xff) / 255.0f}, a{((argb32 >> 24) & 0xff) / 255.0f}
    {}

    Color& ToRGB()
    {
      if(h == 1.0)
        h = 0;
      float c = v * s;
      float h6f = h*6.0f;
      uint32_t h6 = (uint32_t)h6f;

      float f = h6f - h6;

      float p = v * (1 - s);
      float q = v * (1 - s * f);
      float t = v * (1 - s * (1 - f));

      float _v = v;

      switch (h6)
      {
        case 0:
          r = _v; g = t; b = p;
          break;
        case 1:
          r = q; g = _v; b = p;
          break;
        case 2:
          r = p; g = _v; b = t;
          break;
        case 3:
          r = p; g = q; b = _v;
          break;
        case 4:
          r = t; g = p; b = _v;
          break;
        case 5:
          r = _v; g = p; b = q;
          break;
        default:
          r = c; g = c; b = c;
      }
      return *this;
    }

    Color& ToHSV()
    {
      float cmin = r < g ? (r < b ? r : b) : (g < b ? g : b);
      float cmax = r > g ? (r > b ? r : b) : (g > b ? g : b);
      float _r = r;
      float _g = g;
      float _b = b;

      float delta = cmax - cmin;

      v = cmax;
      if (cmax != 0)
        s = delta / cmax;
      if (delta != 0)
      {
        if (cmax == _r)
        {
          h = (_g - _b) / delta * 0.16667;
        }
        else if (cmax == _g)
        {
          h = ((_b - _r) / delta + 2) * 0.16667;
        }
        else // if (cmax == _b)
        {
          h = ((_r - _g) / delta + 4) * 0.16667;
        }

        if(h < 0)
          h++;
      }
      return *this;
    }

    uint32_t AsUInt() const
    {
      return (((uint32_t)(a * 255) & 0xff) << 24) |
        (((uint32_t)(r * 255) & 0xff) << 16) |
        (((uint32_t)(g * 255) & 0xff) << 8) |
        ((uint32_t)(b * 255) & 0xff);
    }

    Color& Invert()
    {
      r = 1 - r;
      g = 1 - g;
      b = 1 - b;
      return *this;
    }

    friend bool operator==(const Color& c1, const Color& c2)
    {
      return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
    }

    friend bool operator!=(const Color& c1, const Color& c2)
    {
      return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b || c1.a != c2.a;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Color& c)
    {
      return stream << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    }
  };
}

