#pragma once

#include <graphics/Skybox.h>
#include <graphics/cameras/Camera3D.h>
#include <graphics/layers/Scene.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/Renderer3D.h>
#include <math/Line.h>

namespace Greet {
  class Layer3D : public Scene
  {
    protected:
      std::vector<Ref<Renderer3D>> renderers;
      Ref<Camera3D> camera;
      Ref<Skybox> skybox;

    public:
      Layer3D(const Ref<Camera3D>& camera, const Ref<Skybox>& skybox);
      Layer3D(const Ref<Camera3D>& camera);

      virtual ~Layer3D();

      virtual void Render() const override;
      virtual void Update(float timeElapsed) override;
      virtual void OnEvent(Event& event) override;
      virtual void ViewportResize(ViewportResizeEvent& event) override;

      void AddRenderer(const Ref<Renderer3D>& renderer);
      void RemoveRenderer(const Ref<Renderer3D>& renderer);

      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const;
      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const;

      Ref<Camera3D>& GetCamera3D() { return camera; }
      const Ref<Camera3D>& GetCamera3D() const { return camera; }
  };
}
