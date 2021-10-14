#pragma once

#include <graphics/gui/Frame.h>
#include <graphics/gui/Text.h>
#include <utils/xml/XMLObject.h>
#include <utils/MetaFile.h>

namespace Greet
{
  class DropDownMenuFrame : public Frame
  {
    static XMLObject frameLook;
    Text text;
    typedef std::function<void(int index)> OnSelectionChanged;
    std::vector<std::string> items;

    Color dropBackgroundColor{0, 0, 0};
    Color dropBorderColor{0, 0, 0};
    TLBR dropBorder{0, 0, 0, 0};
    TLBR dropPadding{0, 0, 0, 0};
    float dropHeight = 30;

    int hoverPos = -1;
    int pressPos = -1;

    OnSelectionChanged callback;

    public:
      DropDownMenuFrame(const XMLObject& xmlObject, const std::vector<std::string>& items);

      void Render(GUIRenderer* renderer) const override;
      void RenderItem(GUIRenderer* renderer, const Text& text, int i, const std::string& styling) const;
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos) override;
      void OnMousePressEventHandler(MousePressEvent& event, const Vec2f& componentPos) override;
      void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2f& componentPos) override;
      void Measure(const Vec2f& emptyParentSpace, const Vec2f& percentageFill) override;

      void SetDropDownItems(const std::vector<std::string>& items);
      void SetOnSelectionChangedCallback(OnSelectionChanged callback);
      void CallOnSelectionChanged(int index);
  };
}
