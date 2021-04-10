#include "Image.h"
#include <graphics/textures/TextureManager.h>

namespace Greet {

  REGISTER_COMPONENT_DEFINITION(Image);

  Image::Image(const std::string& name, Component* parent)
    : Component{name, parent}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"imageColor", &textureColor}},
    });
  }

  Image::Image(const XMLObject& object, Component* parent)
    : Component{object, parent}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"imageColor", &textureColor}},
    });

    std::string formatStr = GUIUtils::GetStringFromXML(object, "format", "fit");
    if(formatStr == "fit")
      format = Format::Fit;
    else if(formatStr == "stretch")
      format = Format::Stretch;
    else if(formatStr == "fill")
      format = Format::Fill;
    else
    {
      Log::Warning("Invalid GUI Image format specified: ", formatStr);
      format = Format::Fit;
    }


    std::string textureName = GUIUtils::GetStringFromXML(object, "image", "");
    if(!textureName.empty())
      texture = TextureManager::LoadTexture2D(textureName);
  }

  void Image::Render(GUIRenderer* renderer) const
  {
    switch(format)
    {
      case Format::Stretch:
      {
        renderer->DrawRect(pos, GetSize(), texture, textureColor, false);
        break;
      }
      case Format::Fit:
      {
        float imageAspect = texture->GetWidth() / (float)texture->GetHeight();
        float componentAspect = GetWidth() / GetHeight();
        Vec2f size = GetSize() * Vec2f{1 / imageAspect, imageAspect};
        if(imageAspect > componentAspect)
          size.h = GetWidth();
        else
          size.w = GetHeight();
        renderer->DrawRect(pos + (GetSize() - size) / 2, size, texture, textureColor, false);
        break;
      }
      case Format::Fill:
      {
        float imageAspect = texture->GetWidth() / (float)texture->GetHeight();
        float componentAspect = GetWidth() / GetHeight();
        Vec2f size = Vec2f{componentAspect, 1 / componentAspect};
        if(imageAspect > componentAspect)
          size.h = 1.0;
        else
          size.w = 1.0;
        renderer->DrawRect(pos, GetSize(), texture, (1.0 - size) / 2.0f, size, textureColor, false);
        break;
      }
    }
  }
}
