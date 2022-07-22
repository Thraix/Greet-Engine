#include "TextureManager.h"

#include <utils/ErrorHandle.h>
#include <utils/MetaFile.h>
#include <graphics/textures/ImageFactory.h>

namespace Greet{

  std::map<std::string, Ref<Texture2D>> TextureManager::texture2Ds;
  std::map<std::string, Ref<CubeMap>> TextureManager::cubeMaps;

  void TextureManager::AddTexture2D(const std::string& id, const Ref<Texture2D>& texture)
  {
    auto it = texture2Ds.find(id);
    if(it != texture2Ds.end())
    {
      Log::Warning("Texture2D already exists: ", id);
      return;
    }
    texture2Ds.emplace(id, texture);
  }

  void TextureManager::AddCubeMap(const std::string& id, const Ref<CubeMap>& texture)
  {
    auto it = cubeMaps.find(id);
    if(it != cubeMaps.end())
    {
      Log::Warning("CubeMap already exists: ", id);
      return;
    }
    cubeMaps.emplace(id, texture);
  }

  TextureParams GetMetaParams(const MetaFileClass& file)
  {
    TextureParams params;
    std::string filter = file.GetValue("filter", "linear");
    if(filter == "none")
      params.filter = TextureFilter::NONE;
    else if(filter == "linear")
      params.filter = TextureFilter::LINEAR;
    else if(filter == "nearest")
      params.filter = TextureFilter::NEAREST;

    std::string wrap = file.GetValue("wrap", "clampEdge");
    if(wrap == "none")
      params.wrap = TextureWrap::NONE;
    else if(wrap == "clampEdge")
      params.wrap = TextureWrap::CLAMP_TO_EDGE;
    else if(wrap == "clampBorder")
      params.wrap = TextureWrap::CLAMP_TO_BORDER;
    else if(wrap == "mirrorRepeat")
      params.wrap = TextureWrap::MIRRORED_REPEAT;
    else if(wrap == "repeat")
      params.wrap = TextureWrap::REPEAT;
    else if(wrap == "mirrorClampEdge")
      params.wrap = TextureWrap::MIRROR_CLAMP_TO_EDGE;

    std::string format = file.GetValue("format", "RGBA");
    if(format == "depth")
      params.internalFormat = TextureInternalFormat::DEPTH_COMPONENT;
    else if(format == "stencil")
      params.internalFormat = TextureInternalFormat::DEPTH_STENCIL;
    else if(format == "red")
      params.internalFormat = TextureInternalFormat::RED;
    else if(format == "rgb")
      params.internalFormat = TextureInternalFormat::RGB;
    else if(format == "rgba")
      params.internalFormat = TextureInternalFormat::RGBA;

    return params;
  }

  Ref<Texture2D> TextureManager::LoadTexture2DUnsafe(const std::string& metaFile)
  {
    auto it = texture2Ds.find(metaFile);
    if(it != texture2Ds.end())
    {
      return it->second;
    }

    MetaFile meta{metaFile};
    if(meta.HasMetaClass("texture2d"))
    {
      const MetaFileClass& texture2d = meta.GetMetaClass("texture2d");
      TextureParams params = GetMetaParams(texture2d);
      return texture2Ds.emplace(metaFile, Texture2D::Create(texture2d.GetValue("filepath", metaFile), params)).first->second;
    }
    return nullptr;
  }

  Ref<CubeMap> TextureManager::LoadCubeMapUnsafe(const std::string& metaFile)
  {
    auto it = cubeMaps.find(metaFile);
    if(it != cubeMaps.end())
    {
      return it->second;
    }

    MetaFile meta{metaFile};
    if(meta.HasMetaClass("cubemap"))
    {
      const MetaFileClass& cubemap = meta.GetMetaClass("cubemap");
      return cubeMaps.emplace(metaFile, NewRef<CubeMap>(cubemap.GetValue("filepath", metaFile))).first->second;
    }
    return nullptr;
  }

  Ref<Texture2D> TextureManager::LoadTexture2D(const std::string& metaFile)
  {
    Ref<Texture2D> texture = LoadTexture2DUnsafe(metaFile);
    if(texture)
      return texture;

    // If the filename is empty, we consider it a normal case to not be able to read the texture.
    // So no error is logged
    if(!metaFile.empty())
    {
      Log::Error("No texture found in meta file: ", metaFile);
    }
    uint width, height;
    ImageData imageData = ImageFactory::GetCantReadImage();
    static Ref<Texture2D> invalid{Texture2D::Create(imageData)};
    return invalid;
  }

  Ref<CubeMap> TextureManager::LoadCubeMap(const std::string& metaFile)
  {
    Ref<CubeMap> cubeMap = LoadCubeMapUnsafe(metaFile);
    if(cubeMap)
      return cubeMap;

    // If the filename is empty, we consider it a normal case to not be able to read the texture.
    // So no error is logged
    if(!metaFile.empty())
    {
      Log::Error("No texture found in meta file: ", metaFile);
    }
    uint width, height;
    ImageData imageData = ImageFactory::GetCantReadImage();
    static Ref<CubeMap> invalid{NewRef<CubeMap>(imageData)};
    return invalid;
  }

  void TextureManager::CleanupUnused()
  {
    for(auto it = cubeMaps.begin(); it != cubeMaps.end();)
    {
      // Only used by the map
      if(it->second.use_count() == 1)
        it = cubeMaps.erase(it);
      else
        ++it;
    }

    for(auto it = texture2Ds.begin(); it != texture2Ds.end();)
    {
      // Only used by the map
      if(it->second.use_count() == 1)
        it = texture2Ds.erase(it);
      else
        ++it;
    }
  }

  void TextureManager::Cleanup()
  {
    cubeMaps.clear();
    texture2Ds.clear();
  }
}
