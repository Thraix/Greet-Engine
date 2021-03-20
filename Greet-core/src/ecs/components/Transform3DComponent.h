#pragma once

#include <math/Mat4.h>
#include <math/Vec3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Transform3DComponent
  {
    private:
      Vec3f position{0, 0, 0};
      Vec3f scale{1, 1, 1};
      Vec3f rotation{0, 0, 0};
    public:
      Mat4 transform;

    Transform3DComponent(const Mat4& transform);
    Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot);
    Transform3DComponent(const MetaFileClass& metaClass);

    void SetPosition(const Vec3f& avPos);
    void SetScale(const Vec3f& avScale);
    void SetRotation(const Vec3f& avRotation);

    const Vec3f& GetPosition() const;
    const Vec3f& GetScale() const;
    const Vec3f& GetRotation() const;

    private:
      void RecalcTransform();
  };
}
