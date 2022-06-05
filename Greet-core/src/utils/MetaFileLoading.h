#pragma once

#include <graphics/Color.h>
#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>
#include <graphics/textures/CubeMap.h>
#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct MetaFileLoading
  {
    MetaFileLoading() = delete;

    static Vec3f LoadVec3f(const MetaFileClass& metaClass, const std::string& key, const Vec3f& defaultValue);
    static Vec2f LoadVec2f(const MetaFileClass& metaClass, const std::string& key, const Vec2f& defaultValue);
    static Vec2f LoadTextureCoord(const MetaFileClass& metaClass, const Ref<Texture2D>& texture, const std::string& key, const Vec2f& defaultValue);
    static float LoadFloat(const MetaFileClass& metaClass, const std::string& key, float defaultValue);
    static int LoadInt(const MetaFileClass& metaClass, const std::string& key, int defaultValue);
    static uint64_t LoadHex64(const MetaFileClass& metaClass, const std::string& key, uint64_t defaultValue);
    static bool LoadBool(const MetaFileClass& metaClass, const std::string& key, bool defaultValue);
    static Color LoadColor(const MetaFileClass& metaClass, const std::string& key, const Color& defaultValue);
    static std::string LoadString(const MetaFileClass& metaClass, const std::string& key, const std::string& defaultValue);
    static Vec2i LoadVec2i(const MetaFileClass& metaClass, const std::string& key, const Vec2i& defaultValue);
    static Ref<Mesh> LoadMesh(const MetaFileClass& metaClass, const std::string& key);
    static Ref<Shader> LoadShader(const MetaFileClass& metaClass, const std::string& defaultShader = "shader3d", const std::string& prefix = "");
    static Ref<Texture2D> LoadTexture2D(const MetaFileClass& metaClass);
    static Ref<CubeMap> LoadCubeMap(const MetaFileClass& metaClass, const std::string& key);
    static Material LoadMaterial(const MetaFileClass& metaClass);
  };
}
