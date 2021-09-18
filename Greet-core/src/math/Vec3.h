#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <logging/Log.h>

#include <math/MathFunc.h>
#include <math/Quaternion.h>
#include <math/Vec4.h>

#include <iostream>

namespace Greet{

  template<typename T>
  struct Vec3
  {
    union
    {
      T vals[3];
      struct {
        T x, y, z;
      };
      struct {
        T r, g, b;
      };
      struct {
        T h, s, v;
      };
    };
    Vec3()
      : x{}, y{}, z{}
    {}

    Vec3(const T& x, const T& y, const T& z)
      : x{x}, y{y}, z{z}
    {}

    Vec3(const T& v)
      : x{v}, y{v}, z{v}
    {}

    template <typename S>
    explicit Vec3(const Vec3<S>& v)
      : x{(S)v.x}, y{(S)v.y}, z{(S)v.z}
    {}

    explicit Vec3(const Vec4<T>& vec4)
      : x{vec4.x}, y{vec4.y}, z{vec4.z}
    {}

    T Length() const
    {
      return sqrt(x * x + y * y + z * z);
    }

    T LengthSQ() const
    {
      return x * x + y * y + z  * z;
    }

    T Dot(const Vec3& vec) const
    {
      return x * vec.x + y * vec.y + z  * vec.z;
    }

    Vec3<T> Cross(const Vec3<T>& vec) const
    {
      return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    // All These modifies the current vec3
    Vec3& Normalize()
    {
      float len = Length();
      if(len == 0)
        return *this;
      return Divide(len);
    }

    // All These modifies the current vec3
    Vec3& ToRadians()
    {
      x = x * M_PI / 180;
      y = y * M_PI / 180;
      z = z * M_PI / 180;
      return *this;
    }

    template <typename std::enable_if<std::is_floating_point<T>::value>* = nullptr>
    Vec3<T>& Rotate(const Vec3& axis, const T& angle)
    {
      Quaternion axisRotation = Quaternion::AxisAngle(axis, angle);
      *this = axisRotation * *this;
      return *this;
    }

    Vec3& Add(const Vec3& other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      return *this;
    }

    Vec3& Subtract(const Vec3& other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      return *this;
    }

    Vec3& Multiply(const Vec3& other)
    {
      x *= other.x;
      y *= other.y;
      z *= other.z;
      return *this;
    }

    Vec3& Divide(const Vec3& other)
    {
      x /= other.x;
      y /= other.y;
      z /= other.z;
      return *this;
    }

    Vec3& Add(const T& other)
    {
      x += other;
      y += other;
      z += other;
      return *this;
    }

    Vec3& Subtract(const T& other)
    {
      x -= other;
      y -= other;
      z -= other;
      return *this;
    }

    Vec3& Multiply(const T& other)
    {
      x *= other;
      y *= other;
      z *= other;
      return *this;
    }

    Vec3& Divide(const T& other)
    {
      x /= other;
      y /= other;
      z /= other;
      return *this;
    }

    bool Compare(const Vec3& other) const
    {
      return x == other.x && y == other.y && z == other.z;
    }

    friend Vec3 operator+(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Add(second);
    }

    friend Vec3 operator-(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Subtract(second);
    }

    friend Vec3 operator*(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Multiply(second);
    }

    friend Vec3 operator/(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Divide(second);
    }

    friend Vec3 operator+(const Vec3& vec, const T& c)
    {
      return Vec3(vec).Add(c);
    }

    friend Vec3 operator-(const Vec3& vec, const T& c)
    {
      return Vec3(vec).Subtract(c);
    }

    friend Vec3 operator*(const Vec3& vec, const T& c)
    {
      return Vec3(vec).Multiply(c);
    }

    friend Vec3 operator/(const Vec3& vec, const T& c)
    {
      return Vec3(vec).Divide(c);
    }

    friend Vec3 operator+(const T& c, const Vec3& vec)
    {
      return Vec3(vec).Add(c);
    }

    friend Vec3 operator-(const T& c, const Vec3& vec)
    {
      return Vec3(c - vec.x,c - vec.y,c - vec.z);
    }

    friend Vec3 operator*(const T& c, const Vec3& vec)
    {
      return Vec3(vec).Multiply(c);
    }

    friend Vec3 operator/(const T& c, const Vec3& vec)
    {
      return Vec3(c / vec.x, c / vec.y, c / vec.z);
    }

    const T& operator[](uint32_t i) const
    {
      ASSERT(i < 3, "Index out of bound");
      return *((&x)+i);
    }

    T& operator[](uint32_t i)
    {
      ASSERT(i < 3, "Index out of bound");
      return *((&x)+i);
    }

    Vec3& operator+=(const Vec3 &other)
    {
      return Add(other);
    }

    Vec3& operator-=(const Vec3 &other)
    {
      return Subtract(other);
    }

    Vec3& operator*=(const Vec3 &other)
    {
      return Multiply(other);
    }

    Vec3& operator/=(const Vec3 &other)
    {
      return Divide(other);
    }

    Vec3& operator+=(const T& c)
    {
      return Add(c);
    }

    Vec3& operator-=(const T& c)
    {
      return Subtract(c);
    }

    Vec3& operator*=(const T& c)
    {
      return Multiply(c);
    }

    Vec3& operator/=(const T& c)
    {
      return Divide(c);
    }

    bool operator!=(const Vec3 &second) const
    {
      return !Compare(second);
    }

    Vec3 operator-() const
    {
      return Vec3(-x, -y, -z);
    }

    bool operator==(const Vec3 &second) const
    {
      return Compare(second);
    }

    bool operator<(const Vec3& rhs) const
    {
      if(x < rhs.x)
        return true;
      else if(x == rhs.x)
      {
        if(y < rhs.y)
          return true;
        else if(y == rhs.y)
          return z < rhs.z;
      }
      return false;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec3& vec)
    {
      return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    }
  };
  namespace Vec
  {
    template <typename T>
    static Vec3<T> Normalize(const Vec3<T>& vec)
    {
      Vec3<T> ret = vec;
      return ret.Normalize();
    }

    template <typename T>
    static T Dot(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
      return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z  * rhs.z;
    }

    template <typename T>
    static Vec3<T> Cross(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
      return Vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }

    template <typename T>
    static Vec3<T> Max(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
      return Vec3<T>{std::max(lhs.x,rhs.x),std::max(lhs.y,rhs.y),std::max(lhs.z,rhs.z)};
    }

    template <typename T>
    static Vec3<T> Min(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
      return Vec3<T>{std::min(lhs.x,rhs.x),std::min(lhs.y,rhs.y),std::min(lhs.z,rhs.z)};
    }

    template <typename T>
    static Vec3<T> Rotate(const Vec3<T>& vector, const Vec3<T>& axis, const T& angle)
    {
      return Vec3<T>{vector}.Rotate(axis, angle);
    }

    template <typename T>
    static float SignedAngle(const Vec3<T>& vec1, const Vec3<T>& vec2, const Vec3<T>& normal)
    {
      float dot = Dot(Normalize(vec1), Normalize(vec2));
      float angle = acos(dot);
      if(dot > 1.0f)
        angle = 0;
      else if(dot < -1.0f)
        angle = M_PI;
      Vec3<T> cross = Cross(vec1, vec2);
      if (Dot(normal, cross) > 0)
        angle = -angle;
      return angle;
    }
  }

  typedef Vec3<float> Vec3f;
  typedef Vec3<int> Vec3i;
}
