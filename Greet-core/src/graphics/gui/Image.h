#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Label.h>

namespace Greet {

  class Image : public Component
  {
    enum class Format {
      Stretch, Fit, Fill
    };

    private:
      REGISTER_COMPONENT_DECLARATION(Image);
    protected:
      Color textureColor = Color{1,1,1};

    public:
      Format format;
      Ref<Texture2D> texture;
      Image(const std::string& name, Component* parent);
      Image(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;
  };
}
