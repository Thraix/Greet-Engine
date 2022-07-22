#pragma once

#include <math/Vec3.h>
#include <math/Vec4.h>

namespace Greet {

  struct Mat4
  {
    union
    {
      float elements[4 * 4];
      struct
      {
        Vec4f columns[4];
      };
    };

    Mat4();
    Mat4(float diagonal);
    Mat4(float* elem);

    const Vec4f& GetColumn(int index) const
    {
      return columns[index];
    }

    void SetColumn(int index, const Vec4f& column)
    {
      columns[index] = column;
    }

    static Mat4 Identity();
    static Mat4 OrthographicViewport(float near, float far);
    static Mat4 Orthographic(float left, float right, float top, float bottom, float near, float far);

    static Mat4 PerspectiveViewport(float fov, float near, float far);
    static Mat4 Perspective(float aspect, float fov, float near, float far);

    static Mat4 TransformationMatrix(const Vec3f& position, const Vec3f& scale, const Vec3f& rotation);
    static Mat4 ViewMatrix(const Vec3f& position, const Vec3f& rotation);
    static Mat4 AlignAxis(const Vec3f& point, const Vec3f& normal, const Vec3f& up);

    /*
       View Matrix of a third person camera around an object.
       position	- Position of the object
       distance	- distance from the object
       height		- height above the object, between -1 and 1
       rotation	- angle around the object
       */
    static Mat4 TPCamera(const Vec3f& position, float distance, float height, float rotation);

    static Mat4 Translate(const Vec3f& translation);
    static Mat4 Scale(const Vec3f& scaling);

    static Mat4 Translate(const float& x, const float& y, const float& z);
    static Mat4 Scale(const float& x, const float& y, const float& z);

    static Mat4 RotateX(float rad);
    static Mat4 RotateY(float rad);
    static Mat4 RotateZ(float rad);
    static Mat4 Rotate(float rad, const Vec3f& axis);

    static Mat4 Inverse(const Mat4& inv);
    static Mat4 InverseTranspose(const Mat4& inv);

    Mat4 Cpy();

    Mat4& Multiply(const Mat4& other);
    Vec3f Multiply(const Vec3f& other) const;
    Vec4f Multiply(const Vec4f& other) const;

    friend Mat4 operator*(Mat4 first, const Mat4 &second);
    Mat4& operator*=(const Mat4 &other);

    friend Vec3f operator*(const Mat4& first, const Vec3f& second);
    friend Vec4f operator*(const Mat4& first, const Vec4f& second);
    friend Mat4 operator~(const Mat4& first);

  };
}
