#include "ECSScene.h"

#include <ecs/components/AnimationComponent.h>
#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Camera3DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/Environment3DComponent.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/NativeScriptComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/ShaderFactory.h>

namespace Greet
{
  ECSScene::ECSScene() :
    ECSScene{NewRef<ECSManager>()}
  {
  }

  ECSScene::ECSScene(const Ref<ECSManager>& ecsManager) :
    manager{ecsManager},
    renderer2d{NewRef<BatchRenderer>(ShaderFactory::Shader2D())},
    framebuffer{NewRef<Framebuffer>(RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight(), true)},
    bloom{NewRef<Bloom>(RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight(), 6)}
  {
  }

  ECSScene::ECSScene(const std::string& scenePath) :
    ECSScene{NewRef<ECSManager>()}
  {
    MetaFile meta{scenePath};
    const std::vector<MetaFileClass>& entities = meta.GetMetaClass("Entity");
    for(auto&& entity : entities)
    {
      if(entity.HasValue("entitypath"))
        LoadEntity(MetaFile{entity.GetValue("entitypath")});
    }
  }

  ECSScene::~ECSScene()
  {
    TextureManager::CleanupUnused();
  }

  void ECSScene::LoadEntity(const MetaFile& meta)
  {
    const std::vector<MetaFileClass>& tag = meta.GetMetaClass("TagComponent");
    if(tag.size() == 0)
    {
      Log::Error("Entity does not contain TagComponent");
      return;
    }
    Entity e = Entity::Create(manager.get());
    e.AddComponent<TagComponent>(tag[0]);

    LoadComponent<Transform3DComponent>(e, meta, "Transform3DComponent");
    LoadComponent<MeshComponent>(e, meta, "MeshComponent");
    LoadComponent<MaterialComponent>(e, meta, "MaterialComponent");
    LoadComponent<Camera3DComponent>(e, meta, "Camera3DComponent");
    LoadComponent<Environment3DComponent>(e, meta, "Environment3DComponent");

    LoadComponent<AnimationComponent>(e, meta, "AnimationComponent");
    LoadComponent<Transform2DComponent>(e, meta, "Transform2DComponent");
    LoadComponent<SpriteComponent>(e, meta, "SpriteComponent");
    LoadComponent<Camera2DComponent>(e, meta, "Camera2DComponent");
    LoadComponent<Environment2DComponent>(e, meta, "Environment2DComponent");
    LoadComponent<ColorComponent>(e, meta, "ColorComponent");

    LoadComponent<NativeScriptComponent>(e, meta, "NativeScriptComponent");

    LoadExtComponents(e, meta);
  }

  Entity ECSScene::AddEntity(const std::string& tag)
  {
    Entity e = Entity::Create(manager.get());
    e.AddComponent<TagComponent>(tag);
    return e;
  }

  void ECSScene::RemoveEntity(const Entity& entity)
  {
    manager->DestroyEntity(entity.GetID());
  }

  void ECSScene::RemoveEntity(EntityID entity)
  {
    manager->DestroyEntity(entity);
  }

  void ECSScene::Render() const
  {
    Render3DScene();
    Render2D();
  }

  void ECSScene::Render2D() const
  {
    Entity camera{manager.get()};
    manager->Each<Camera2DComponent>([&](EntityID id, Camera2DComponent& cam)
    {
      if(cam.active)
      {
        if(camera)
          Log::Warning("More than one active 2D camera in scene");
        camera.SetID(id);
      }
    });

    // No camera found, so cannot render anything
    if(!camera)
      return;

    Entity environment{manager.get()};
    manager->Each<Environment2DComponent>([&](EntityID id, Environment2DComponent& env)
        {
          if(environment)
            Log::Warning("More than one environment in 2D scene");
          environment.SetID(id);
        });

    Camera2DComponent& cam = camera.GetComponent<Camera2DComponent>();
    if(environment)
      renderer2d->SetShader(environment.GetComponent<Environment2DComponent>().shader);

    renderer2d->Begin();
    cam.SetShaderUniforms(renderer2d->GetShader());
    manager->Each<Transform2DComponent>([&](EntityID id, Transform2DComponent& transform)
    {
      Entity e{manager.get(), id};
      if(e.HasComponent<SpriteComponent>())
      {
        SpriteComponent& sprite = e.GetComponent<SpriteComponent>();
        renderer2d->DrawRect(transform.transform, sprite.texture, sprite.GetTexPos(), sprite.GetTexSize());
      }
      else if(e.HasComponent<ColorComponent>())
      {
        renderer2d->DrawRect(transform.transform);
        ColorComponent& color = e.GetComponent<ColorComponent>();
        renderer2d->DrawRect(transform.transform, color.color);
      }
    });
    renderer2d->End();
    renderer2d->Flush();
  }

  void ECSScene::Render3DScene() const
  {
    Entity camera{manager.get()};
    camera = manager->Find<Camera3DComponent>(
      [&](EntityID id, Camera3DComponent& cam)
      {
        return cam.active;
      });

    if(!camera)
    {
      return;
    }

    Entity environment{manager.get()};
    environment = manager->Find<Environment3DComponent>(
      [&](EntityID id, Environment3DComponent& env)
      {
        return true;
      });

    Camera3DComponent& cam = camera.GetComponent<Camera3DComponent>();
    const Environment3DComponent* env = &defaultEnv;
    if(environment)
      env = &environment.GetComponent<Environment3DComponent>();

    Render3DBegin(cam, *env);
    Render3D(cam, *env);
    Render3DEnd(cam, *env);
  }

  void ECSScene::Render3DBegin(const Camera3DComponent& cam, const Environment3DComponent& env) const
  {
    framebuffer->Bind();
    env.Skybox(cam);
  }

  void ECSScene::Render3D(const Camera3DComponent& cam, const Environment3DComponent& env) const
  {
    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>(
      [&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
      {
        material.material.Bind();
        cam.SetShaderUniforms(material.material.GetShader());
        material.material.GetShader()->SetUniformMat4("uTransformationMatrix", transform.transform);
        material.material.GetShader()->SetUniform3f("uLightPosition", Vec3f(30.0f, 50.0f, 40.0f));
        material.material.GetShader()->SetUniform3f("uLightColor", Vec3f(0.7, 0.7, 0.7));
        env.SetShaderUniforms(material.material.GetShader());
        mesh.mesh->Bind();
        mesh.mesh->Render();
        mesh.mesh->Unbind();
        material.material.Unbind();
      });
  }

  void ECSScene::Render3DEnd(const Camera3DComponent& cam, const Environment3DComponent& env) const
  {
    framebuffer->Unbind();
    bloom->Render(framebuffer->GetColorTexture(), env);
  }

  void ECSScene::Update(float timeElapsed)
  {
    manager->Each<NativeScriptComponent>([&](EntityID id, NativeScriptComponent& script)
    {
      if(!script.script->HasBoundEntity())
        script.BindEntity({manager.get(), id});
      if(!script.created)
      {
        script.Create();
      }
      script.Update(timeElapsed);
    });
    manager->Each<AnimationComponent, SpriteComponent>([&](EntityID id, AnimationComponent& animation, SpriteComponent& sprite) {
      animation.timeElapsed += timeElapsed;
      if(animation.timeElapsed > animation.keytime)
      {
        animation.timeElapsed -= animation.keytime;
        animation.aminationIndex++;
        animation.aminationIndex %= animation.animationCount;
        // TODO: Figure out a better way of handling the spriteSheet position
        sprite.spriteSheetX = animation.aminationIndex;
      }
    });
  }

  void ECSScene::OnEvent(Greet::Event& event)
  {
    if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
    {
      ViewportResizeEvent& e = static_cast<ViewportResizeEvent&>(event);
      framebuffer->Resize(e.GetWidth(), e.GetHeight());
      bloom->Resize(e.GetWidth(), e.GetHeight());
      manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
      {
        cam.ViewportResize(e);
      });
      manager->Each<Camera2DComponent>([&](EntityID id, Camera2DComponent& cam)
      {
        cam.ViewportResize(e);
      });
    }
    manager->Each<NativeScriptComponent>([&](EntityID id, NativeScriptComponent& script)
    {
      if(script.created)
        script.OnEvent(event);
    });
  }
}
