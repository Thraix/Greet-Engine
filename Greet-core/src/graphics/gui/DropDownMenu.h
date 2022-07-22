#pragma once

#include <graphics/gui/DropDownMenuFrame.h>
#include <graphics/gui/Component.h>
#include <graphics/gui/Text.h>

namespace Greet {
  class DropDownMenu : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(DropDownMenu);
    private:
      //////////////////
      // Style variables
      Text text;
      Color dropBackgroundColor{0, 0, 0};
      Color dropBorderColor{0, 0, 0};
      TLBR dropBorder{0, 0, 0, 0};
      TLBR dropPadding{0, 0, 0, 0};
      std::string label;

      typedef std::function<void(Component* component, const std::string& oldSelection, const std::string& newSelection)> OnSelectionChanged;
      OnSelectionChanged selectionChangedCallback;
      std::vector<std::string> items{};
      int selectedIndex = 0;
      bool opened;

      DropDownMenuFrame* frame;

    public:
      DropDownMenu(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos) override;
      void LoadFrameStyle(const MetaFile& metaFile) override;

      DropDownMenu& SetDropDownItems(const std::vector<std::string>& items);
      const std::vector<std::string>& GetDropDownItems() const;

      void SetOnSelectionChangeCallback(OnSelectionChanged callback);
      void CallOnSelectionChanged(int index);

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;
      const Color& GetColor() const;
      const Font& GetFont() const;
      float GetFontSize() const;

      DropDownMenu& SetGravity(Text::Gravity grav);
      DropDownMenu& SetAlign(Text::Align align);
      DropDownMenu& SetFont(const std::string& font);
      DropDownMenu& SetFontSize(float fontSize);
      DropDownMenu& SetColor(const Color& color);

  };
}
