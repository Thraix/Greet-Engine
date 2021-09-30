#pragma once

#include <ecs/components/Camera3DComponent.h>
#include <graphics/models/Mesh.h>
#include <graphics/shaders/Shader.h>
#include <graphics/textures/CubeMap.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Environment3DComponent
  {
    private:
      Ref<Mesh> skyboxMesh;
    public:
      Ref<Shader> skybox;
      Ref<CubeMap> skyboxTexture;
      Vec3f lightPos = Vec3f(30.0, 20.0, 40.0);
      Color lightColor = Color{1.0f, 0.96f, 0.9f};
      float fogNearDistance = 100;
      float fogFarDistance = 140;
      Color fogColor = Color{0.125f, 0.125f, 0.125f};

      Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture, const Vec3f& lightPos, const Color& lightColor, float fogNearDistance, float fogFarDistance, const Color& fogColor);
      Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture);
      Environment3DComponent(const Ref<Shader>& skybox);
      Environment3DComponent(const Ref<CubeMap>& skyboxTexture);
      Environment3DComponent();
      Environment3DComponent(const MetaFileClass& metaClass);

      void Skybox(Camera3DComponent& camera);
      void SetShaderUniforms(const Ref<Shader>& shader);
  };
}
