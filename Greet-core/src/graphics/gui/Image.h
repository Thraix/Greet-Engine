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
      virtual ~Image() = default;
      void Render(GUIRenderer* renderer) const override;

      void SetTexture(const std::string& metaFile);
      void SetTexture(const Ref<Texture2D>& texture);

      float GetWrapWidth() const override;
      float GetWrapHeight() const override;

  };
}
