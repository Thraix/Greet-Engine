#include "ECSScene.h"

#include <fstream>

#include <ecs/components/AnimationComponent.h>
#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Camera3DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/Environment3DComponent.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/NativeScriptComponent.h>
#include <ecs/components/SerializeComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/ShaderFactory.h>
#include <utils/UUID.h>
#include <utils/Utils.h>

namespace Greet
{
  template <typename T>
  static void LoadComponent(Entity& entity, const MetaFile& meta, const std::string& componentName)
  {
    if(meta.HasMetaClass(componentName))
    {
      const MetaFileClass& metaClass = meta.GetMetaClass(componentName);
      if(entity.HasComponent<T>())
      {
        entity.GetComponent<T>() = T{metaClass};
      }
      else
      {
        entity.AddComponent<T>(metaClass);
      }
    }
  }

  template <typename T>
  static void SaveComponent(Entity& entity, MetaFile& meta)
  {
    if(entity.HasComponent<T>())
    {
      meta << entity.GetComponent<T>();
    }
  }

  ECSScene::ECSScene() :
    ECSScene{NewRef<ECSManager>()}
  {
  }

  ECSScene::ECSScene(const Ref<ECSManager>& ecsManager)
    : ECSScene{ecsManager, ""}
  {}

  ECSScene::ECSScene(const std::string& scenePath) :
    ECSScene{NewRef<ECSManager>(), scenePath}
  {}

  ECSScene::ECSScene(const Ref<ECSManager>& ecsManager, const std::string& scenePath)
    : manager{ecsManager},
    renderer2d{NewRef<BatchRenderer>(ShaderFactory::Shader2D())},
    framebuffer{NewRef<Framebuffer>(RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight(), true)},
    bloom{NewRef<Bloom>(RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight(), 6)}
  {
    if(!scenePath.empty())
    {
      std::vector<MetaFile> entities = MetaFile::ReadList(scenePath);
      for(auto&& entity : entities)
      {
        Entity e = Entity::Create(manager.get());;
        LoadEntity(entity, e);
        e.AddComponent<SerializeComponent>();
        if(!e.HasComponent<TagComponent>())
        {
          UUID uuid;
          Log::Warning("Entity does not contain TagComponent");
          Log::Warning("Assigning a name=%s", uuid);
          e.AddComponent<TagComponent>(uuid.GetString());
        }
        Log::Info("Loaded entity: %s", e.GetComponent<TagComponent>().tag);
      }
    }
  }


  ECSScene::~ECSScene()
  {
    TextureManager::CleanupUnused();
  }

  // Cyclic Blueprints will currently infinite loop, so depth is used to avoid infinite recursion
  void ECSScene::LoadEntity(const MetaFile& meta, Entity entity, int depth)
  {
    const int MAX_RECURSION = 10;
    if(depth >= MAX_RECURSION)
    {
      Log::Warning("Maximum blueprint recursion (%s) reached (cyclic dependency?)", MAX_RECURSION);
      return;
    }
    if(meta.HasMetaClass("Blueprint"))
    {
      const MetaFileClass& blueprint = meta.GetMetaClass("Blueprint");
      if(blueprint.HasValue("path"))
      {
        LoadEntity(MetaFile{blueprint.GetValue("path")}, entity, depth + 1);
      }
    }

    LoadComponent<TagComponent>(entity, meta, "TagComponent");

    LoadComponent<Transform3DComponent>(entity, meta, "Transform3DComponent");
    LoadComponent<MeshComponent>(entity, meta, "MeshComponent");
    LoadComponent<MaterialComponent>(entity, meta, "MaterialComponent");
    LoadComponent<Camera3DComponent>(entity, meta, "Camera3DComponent");
    LoadComponent<Environment3DComponent>(entity, meta, "Environment3DComponent");

    LoadComponent<AnimationComponent>(entity, meta, "AnimationComponent");
    LoadComponent<Transform2DComponent>(entity, meta, "Transform2DComponent");
    LoadComponent<SpriteComponent>(entity, meta, "SpriteComponent");
    LoadComponent<Camera2DComponent>(entity, meta, "Camera2DComponent");
    LoadComponent<Environment2DComponent>(entity, meta, "Environment2DComponent");
    LoadComponent<ColorComponent>(entity, meta, "ColorComponent");

    LoadComponent<NativeScriptComponent>(entity, meta, "NativeScriptComponent");

    if(depth == 0)
    {
      if(meta.HasMetaClass("UUIDComponent"))
      {
        LoadComponent<UUIDComponent>(entity, meta, "UUIDComponent");
      }
      else
      {
        UUID uuid{};
        Log::Warning("No UUID specified for entity, generating one: %s", uuid);
        if(entity.HasComponent<UUIDComponent>())
        {
          entity.GetComponent<UUIDComponent>() = UUIDComponent{uuid};
        }
        else
        {
          entity.AddComponent<UUIDComponent>(uuid);
        }
      }
    }

    LoadExtComponents(entity, meta);
  }

  void ECSScene::Serialize(const std::string& path)
  {
    std::vector<MetaFile> entities;
    manager->Each<SerializeComponent>([&](EntityID entityId, SerializeComponent& serialize){
      if(serialize.serialize)
      {
        Entity entity{manager.get(), entityId};
        MetaFile& file = entities.emplace_back(MetaFile{});
        SaveComponent<AnimationComponent>(entity, file);
        SaveComponent<Camera2DComponent>(entity, file);
        SaveComponent<ColorComponent>(entity, file);
        SaveComponent<Environment2DComponent>(entity, file);
        SaveComponent<SpriteComponent>(entity, file);
        SaveComponent<TagComponent>(entity, file);
        SaveComponent<Transform2DComponent>(entity, file);
        SaveComponent<UUIDComponent>(entity, file);
        SaveComponent<NativeScriptComponent>(entity, file);
      }
    });
    std::ofstream of{path};
    for(auto& entity : entities)
    {
      of << entity << "---" << std::endl;
    }
  }

  Entity ECSScene::AddEntity(const std::string& tag)
  {
    Entity e = Entity::Create(manager.get());
    e.AddComponent<UUIDComponent>(UUID{});
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
    Entity camera = GetCamera2DEntity();

    // No camera found, so cannot render anything
    if(!camera)
      return;

    Entity environment = GetEnvironment2DEntity();

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
        Color color{1, 1, 1};
        if(e.HasComponent<ColorComponent>())
        {
          color = e.GetComponent<ColorComponent>().color;
        }
        SpriteComponent& sprite = e.GetComponent<SpriteComponent>();
        renderer2d->DrawRect(transform.GetTransform(), sprite.texture, sprite.GetTexPos(), sprite.GetTexSize(), color.AsUInt());
      }
      else if(e.HasComponent<ColorComponent>())
      {
        Mat3 matrix = transform.GetTransform();
        renderer2d->DrawRect(matrix);
        ColorComponent& color = e.GetComponent<ColorComponent>();
        renderer2d->DrawRect(matrix, color.color);
      }
    });
    renderer2d->End();
    renderer2d->Flush();
  }

  void ECSScene::Render3DScene() const
  {
    Entity camera = GetCamera3DEntity();

    if(!camera)
    {
      return;
    }

    Entity environment = GetEnvironment3DEntity();

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
        sprite.spriteSheetPos.x = animation.aminationIndex;
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

  Entity ECSScene::GetCamera2DEntity() const
  {
    Entity camera{manager.get()};
    camera = manager->Find<Camera2DComponent>([&](EntityID id, Camera2DComponent& cam)
    {
      return cam.active;
    });
    return camera;
  }

  Entity ECSScene::GetCamera3DEntity() const
  {
    Entity camera{manager.get()};
    camera = manager->Find<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      return cam.active;
    });
    return camera;
  }

  Entity ECSScene::GetEnvironment2DEntity() const
  {
    Entity environment{manager.get()};
    environment = manager->Find<Environment2DComponent>([&](EntityID id, Environment2DComponent& env)
    {
      return true;
    });
    return environment;
  }

  Entity ECSScene::GetEnvironment3DEntity() const
  {
    Entity environment{manager.get()};
    environment = manager->Find<Environment3DComponent>(
      [&](EntityID id, Environment3DComponent& env)
      {
        return true;
      });
    return environment;
  }
}
