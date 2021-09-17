#include "MetaFileLoading.h"

#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/models/MeshFactory.h>
#include <utils/OBJUtils.h>

#include <sstream>

namespace Greet
{
    Vec3f MetaFileLoading::LoadVec3f(const MetaFileClass& metaClass, const std::string& key, const Vec3f& defaultValue)
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

    Vec2f MetaFileLoading::LoadVec2f(const MetaFileClass& metaClass, const std::string& key, const Vec2f& defaultValue)
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

    Vec2f MetaFileLoading::LoadTextureCoord(const MetaFileClass& metaClass, const Ref<Texture2D>& texture, const std::string& key, const Vec2f& defaultValue)
    {
      Vec2f vec = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        int x, y;
        ss >> x >> y;
        vec.x = x / (float)texture->GetWidth();
        vec.y = y / (float)texture->GetWidth();
        return vec;
      }
      return vec;
    }

    float MetaFileLoading::LoadFloat(const MetaFileClass& metaClass, const std::string& key, float defaultValue)
    {
      float f = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        ss >> f;
        return f;
      }
      return f;
    }

    bool MetaFileLoading::LoadBool(const MetaFileClass& metaClass, const std::string& key, bool defaultValue)
    {
      bool b = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::string s{metaClass.GetValue(key)};
        b = s == "true" ? true : false;
        return b;
      }
      return b;
    }

    Color MetaFileLoading::LoadColor(const MetaFileClass& metaClass, const std::string& key, const Color& defaultValue)
    {
      Color color = defaultValue;
      if(metaClass.HasValue(key))
      {
        std::stringstream ss{metaClass.GetValue(key)};
        ss >> color.r >> color.g >> color.b;
        if(!ss.eof())
          ss >> color.a;
        return color;
      }
      return color;
    }

    std::string MetaFileLoading::LoadString(const MetaFileClass& metaClass, const std::string& key, const std::string& defaultValue)
    {
      if(metaClass.HasValue(key))
      {
        return metaClass.GetValue(key);
      }
      return defaultValue;
    }

    Vec2i MetaFileLoading::LoadVec2i(const MetaFileClass& metaClass, const std::string& key, const Vec2i& defaultValue)
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

    Ref<Mesh> MetaFileLoading::LoadMesh(const MetaFileClass& metaClass, const std::string& key)
    {
      if(metaClass.HasValue(key))
      {
        const std::string& type = metaClass.GetValue(key);
        if(type == "cube")
          return Ref<Mesh>(new Mesh{MeshFactory::Cube()});
        if(type == "sphere")
          return Ref<Mesh>(new Mesh{MeshFactory::Sphere()});
        if(type == "tetrahedron")
          return Ref<Mesh>(new Mesh{MeshFactory::Tetrahedron()});
        if(type == "plane")
          return Ref<Mesh>(new Mesh{MeshFactory::Plane()});
        if(type == "grid")
          return Ref<Mesh>(new Mesh{MeshFactory::Grid(
                LoadVec2i(metaClass, key+"-gridSize", {32,32}),
                {},
                LoadVec2f(metaClass, key+"-gridScale", {1, 1})
                )});
        if(type == "model")
          return Ref<Mesh>(new Mesh(OBJUtils::LoadObj(LoadString(metaClass, key + "-path", ""))));
      }
      return Ref<Mesh>(new Mesh{MeshFactory::Cube()});
    }

    Ref<Shader> MetaFileLoading::LoadShader(const MetaFileClass& metaClass, const std::string& defaultShader, const std::string& prefix)
    {
      std::string typeKey = prefix == "" ? "shadertype" : prefix + "-shadertype";
      if(metaClass.HasValue(typeKey))
      {
        const std::string& type = metaClass.GetValue(typeKey);
        if(type == "shader3d")
          return ShaderFactory::Shader3D();
        else if(type == "shader2d")
          return ShaderFactory::Shader2D();
        else if(type == "skybox")
          return ShaderFactory::ShaderSkybox();
        else if(type == "gui")
          return ShaderFactory::ShaderGUI();
        else if(type == "file")
        {
          std::string pathKey = prefix == "" ? "shaderpath" : prefix + "-shaderpath";
          return Shader::FromFile(LoadString(metaClass, pathKey, ""));
        }
      }
      if(defaultShader == "shader3d")
        return ShaderFactory::Shader3D();
      else if(defaultShader == "shader2d")
        return ShaderFactory::Shader2D();
      else if(defaultShader == "skybox")
        return ShaderFactory::ShaderSkybox();
      else if(defaultShader == "gui")
        return ShaderFactory::ShaderGUI();
      return ShaderFactory::Shader3D();
    }

    Ref<Texture2D> MetaFileLoading::LoadTexture2D(const MetaFileClass& metaClass)
    {
      if(metaClass.HasValue("texture"))
      {
        const std::string& metaPath = metaClass.GetValue("texture");
        return TextureManager::LoadTexture2D(metaPath);
      }
      return nullptr;
    }

    Ref<CubeMap> MetaFileLoading::LoadCubeMap(const MetaFileClass& metaClass, const std::string& key)
    {
      if(metaClass.HasValue(key))
      {
        const std::string& metaPath = metaClass.GetValue(key);
        return TextureManager::LoadCubeMap(metaPath);
      }
      return nullptr;
    }

    Material MetaFileLoading::LoadMaterial(const MetaFileClass& metaClass)
    {
      Material mat{LoadShader(metaClass), LoadTexture2D(metaClass)};
      mat.SetColor(LoadColor(metaClass, "color", Color{1, 1, 1}));
      mat.SetAmbient(LoadFloat(metaClass, "ambient", 0.3));
      mat.SetIntensity(LoadFloat(metaClass, "intensity", 1.0));
      mat.SetSpecularExponent(LoadFloat(metaClass, "specularExponent", 10.0));
      mat.SetSpecularStrength(LoadFloat(metaClass, "specularStrength", 0.5));
      mat.SetDiffuseStrength(LoadFloat(metaClass, "diffuseStrength", 0.5));
      return mat;
    }
}
