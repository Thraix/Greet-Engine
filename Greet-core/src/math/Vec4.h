#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <logging/Log.h>

#include <iostream>

namespace Greet{

  template <typename T>
  struct Vec4
  {
    union
    {
      T vals[4];
      struct {
        T x, y, z, w;
      };
      struct {
        T r, g, b, a;
      };
      struct {
        T h, s, v, hsv_a;
      };
      struct {
        T top, left, bottom, right;
      };
    };
    Vec4()
      : x{0}, y{0}, z{0}, w{0}
    {}

    Vec4(const T& f)
      : x{f}, y{f}, z{f}, w{f}
    {}

    Vec4(const T& x, const T& y, const T& z, const T& w)
      : x{x}, y{y}, z{z}, w{w}
    {}

    template <typename S>
    explicit Vec4(const Vec4<S>& v)
      : x{(S)v.x}, y{(S)v.y}, z{(S)v.z}, w{(S)v.w}
    {}

    T Length() const
    {
      return sqrt(x*x + y*y + z*z+ w*w);
    }

    T LengthSQ() const
    {
      return x*x + y*y + z*z+ w*w;
    }

    Vec4& Add(const Vec4& other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      w += other.w;
      return *this;
    }

    Vec4& Subtract(const Vec4& other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      w -= other.w;
      return *this;
    }

    Vec4& Multiply(const Vec4& other)
    {
      x *= other.x;
      y *= other.y;
      z *= other.z;
      w *= other.w;
      return *this;
    }

    Vec4& Divide(const Vec4& other)
    {
      x /= other.x;
      y /= other.y;
      z /= other.z;
      w /= other.w;
      return *this;
    }

    Vec4& Add(const T& c)
    {
      x += c;
      y += c;
      z += c;
      w += c;
      return *this;
    }

    Vec4& Subtract(const T& c)
    {
      x -= c;
      y -= c;
      z -= c;
      w -= c;
      return *this;
    }

    Vec4& Multiply(const T& c)
    {
      x *= c;
      y *= c;
      z *= c;
      w *= c;
      return *this;
    }

    Vec4& Divide(const T& c)
    {
      x /= c;
      y /= c;
      z /= c;
      w /= c;
      return *this;
    }

    bool Compare(const Vec4& other)
    {
      return x == other.x& & y == other.y& & z == other.z& & w == other.w;
    }

    friend Vec4 operator+(const Vec4& first, const Vec4& second)
    {
      return Vec4(first).Add(second);
    }

    friend Vec4 operator-(const Vec4& first, const Vec4& second)
    {
      return Vec4(first).Subtract(second);
    }

    friend Vec4 operator*(const Vec4& first, const Vec4& second)
    {
      return Vec4(first).Multiply(second);
    }

    friend Vec4 operator/(const Vec4& first, const Vec4& second)
    {
      return Vec4(first).Divide(second);
    }

    friend Vec4 operator+(const Vec4& first, const T& c)
    {
      return Vec4(first).Add(c);
    }

    friend Vec4 operator-(const Vec4& first, const T& c)
    {
      return Vec4(first).Subtract(c);
    }

    friend Vec4 operator*(const Vec4&  first, const T& c)
    {
      return Vec4(first).Multiply(c);
    }

    friend Vec4 operator/(const Vec4& first, const T& c)
    {
      return Vec4(first).Divide(c);
    }

    T& operator[](uint32_t i)
    {
      ASSERT(i < 4, "Index out of bound");
      return *((&x)+i);
    }

    const T& operator[](uint32_t i) const
    {
      ASSERT(i < 4, "Index out of bound");
      return *((&x)+i);
    }

    Vec4& operator+=(const Vec4& other)
    {
      return Add(other);
    }

    Vec4& operator-=(const Vec4& other)
    {
      return Subtract(other);
    }

    Vec4& operator*=(const Vec4& other)
    {
      return Multiply(other);
    }

    Vec4& operator/=(const Vec4& other)
    {
      return Divide(other);
    }

    bool operator==(const Vec4& other)
    {
      return Compare(other);
    }

    bool operator!=(const Vec4& other)
    {
      return !Compare(other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec4& vec)
    {
      return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    }
  };

  typedef Vec4<float> Vec4f;
  typedef Vec4<int> Vec4i;
}
