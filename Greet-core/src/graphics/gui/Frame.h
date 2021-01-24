#pragma once

#include <vector>
#include <graphics/gui/Container.h>
#include <graphics/gui/GUIMouseListener.h>
#include <utils/xml/XML.h>

namespace Greet {
  class Frame : public Container
  {
    private:
      REGISTER_FRAME_DECLARATION(Frame);

    protected:

      static byte RESIZING_RIGHT;
      static byte RESIZING_LEFT;
      static byte RESIZING_TOP;
      static byte RESIZING_BOTTOM;
      static int RESIZING_MARGIN;

      bool m_resizing;
      byte m_resizableFlags;
      byte m_resizingFlags;
      Vec2f minSize;

      bool m_stayInsideWindow;
      bool shouldCloseUnfocus;

      Vec2f m_clickPos;
      Vec2f m_posOrigin;
      Vec2f m_sizeOrigin;

      GUIMouseListener* m_mouseListener;
      MetaFile styleFile;

    public:
      bool visible;

    public:
      Frame();
      Frame(const XMLObject& object);
      virtual ~Frame();

      virtual bool IsMouseInside(const Vec2f& mousePos) const override;
      virtual void SetPosition(const Vec2f& pos) override;

      // Check if the mouse is within the resize window
      bool IsHoverResize(const Vec2f& mousePos);
      void SetResizeFlags(const Vec2f& mousePos);

      // Resize the window the mouse position
      void Resize(const Vec2f& mousePos);

      // Clamp the container inside window after resizing it.
      void ResizeScreenClamp();

      void OnMousePressEventHandler(MousePressEvent& event, const Vec2f& componentPos) override;
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos) override;
      void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2f& componentPos) override;

      // Getters and setters
      virtual Vec2f GetComponentPosition() const { return Vec2f(margin.left + border.left, margin.top + border.top); };
      virtual Vec2f GetComponentSize() const { return GetSize() - GetComponentPosition() - Vec2f(margin.right + border.right, margin.bottom + border.bottom); }

      // Set listeners
      void SetGUIMouseListener(GUIMouseListener* listener);

      // Listeners
      virtual void OnViewportResize(int width, int height);

      virtual void ChildChangedFocus(bool focused) override;

      const MetaFile& GetStylingFile() const
      {
        return styleFile;
      }

  };
}
