#pragma once

#include <math/Vec2.h>
#include <math/Mat3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Transform2DComponent
  {
    private:
      Vec2f position{0, 0};
      Vec2f scale{1, 1};
      float rotation{0};
    public:
      Mat3 transform;
      Transform2DComponent(const Mat3& transform);
      Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot);
      Transform2DComponent(const MetaFileClass& metaClass);

      void SetPosition(const Vec2f& avPos);
      void SetScale(const Vec2f& avScale);
      void SetRotation(float avRotation);

      const Vec2f& GetPosition() const;
      const Vec2f& GetScale() const;
      float GetRotation() const;

      private:
        void RecalcTransform();
  };
}
