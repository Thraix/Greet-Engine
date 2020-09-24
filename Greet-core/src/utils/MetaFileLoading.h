#pragma once

#include <utils/MetaFile.h>
#include <math/Vec3.h>
#include <graphics/models/Mesh.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/models/Material.h>
#include <graphics/textures/TextureManager.h>
#include <utils/OBJUtils.h>
#include <graphics/textures/ImageFactory.h>

namespace Greet
{
  struct MetaFileLoading
  {
    MetaFileLoading() = delete;

    static Vec3f LoadVec3f(const MetaFileClass& metaClass, const std::string& key, const Vec3f& defaultValue)
    {
      Vec3f vec = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        ss >> vec.x >> vec.y >> vec.z;
        return vec;
      }
      return vec;
    }

    static Vec2f LoadVec2f(const MetaFileClass& metaClass, const std::string& key, const Vec2f& defaultValue)
    {
      Vec2f vec = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        ss >> vec.x >> vec.y;
        return vec;
      }
      return vec;
    }

    static std::string LoadString(const MetaFileClass& metaClass, const std::string& key, const std::string& defaultValue)
    {
      if(metaClass.HasValue(key))
      {
        return metaClass.GetValue(key);
      }
      return defaultValue;
    }

    static Vec2i LoadVec2i(const MetaFileClass& metaClass, const std::string& key, const Vec2i& defaultValue)
    {
      Vec2i vec = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        ss >> vec.x >> vec.y;
        return vec;
      }
      return vec;
    }

    static Ref<Mesh> LoadMesh(const MetaFileClass& metaClass, const std::string& key)
    {
      if(metaClass.HasValue(key))
      {
        const std::string& type = metaClass.GetValue(key);
        if(type == "cube")
          return Ref<Mesh>(new Mesh{MeshFactory::Cube()});
        if(type == "sphere")
          return Ref<Mesh>(new Mesh{MeshFactory::Sphere()});
        if(type == "Tetrahedron")
          return Ref<Mesh>(new Mesh{MeshFactory::Tetrahedron()});
        if(type == "Plane")
          return Ref<Mesh>(new Mesh{MeshFactory::Plane()});
        if(type == "Grid")
          return Ref<Mesh>(new Mesh{MeshFactory::Grid(LoadVec2i(metaClass, key+"-gridSize", {32,32}))});
        if(type == "model")
          return Ref<Mesh>(new Mesh(OBJUtils::LoadObj(LoadString(metaClass, key + "-path", ""))));
      }
      return Ref<Mesh>(new Mesh{MeshFactory::Cube()});
    }

    static Ref<Shader> LoadShader(const MetaFileClass& metaClass)
    {
      if(metaClass.HasValue("shadertype"))
      {
        const std::string& type = metaClass.GetValue("shadertype");
        if(type == "shader3d")
          return ShaderFactory::Shader3D();
        else if(type == "shader2d")
          return ShaderFactory::Shader2D();
        else if(type == "skybox")
          return ShaderFactory::ShaderSkybox();
        else if(type == "gui")
          return ShaderFactory::ShaderGUI();
        else if(type == "file")
          return Shader::FromFile(LoadString(metaClass, "shaderpath", ""));
      }
      return ShaderFactory::Shader3D();
    }

    static Ref<Texture2D> LoadTexture2D(const MetaFileClass& metaClass)
    {
      if(metaClass.HasValue("texture"))
      {
        const std::string& metaPath = metaClass.GetValue("texture");
        return TextureManager::LoadTexture2D(metaPath);
      }
      return nullptr;
    }

    static Ref<Material> LoadMaterial(const MetaFileClass& metaClass)
    {
      return Ref<Material>(new Material(LoadShader(metaClass), LoadTexture2D(metaClass)));
    }
  };
}
