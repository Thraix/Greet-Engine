#pragma once

#include <graphics/fonts/Font.h>
#include <graphics/gui/Component.h>
#include <graphics/gui/Label.h>
#include <graphics/gui/Text.h>

namespace Greet {
  class Label : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Label);
    private:
      //////////////////
      // Style variables
      Text text;

      bool hasMaxWidth;
      float maxWidth;

    public:
      Label(const std::string& name, Component* parent);
      Label(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize);
      Label(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;

      Label& SetText(const std::string& text);
      const std::string& GetText() const;

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;
      const Color& GetColor() const;
      const Font& GetFont() const;
      float GetFontSize() const;

      Label& SetGravity(Text::Gravity grav);
      Label& SetAlign(Text::Align align);
      Label& SetFont(const std::string& font);
      Label& SetFontSize(float fontSize);
      Label& SetColor(const Color& color);

  };
}
