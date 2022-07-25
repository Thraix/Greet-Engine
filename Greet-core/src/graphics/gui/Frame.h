#pragma once

#include <graphics/gui/Container.h>
#include <graphics/gui/GUIMouseListener.h>
#include <utils/xml/XMLObject.h>

namespace Greet {
  class Frame : public Container
  {
    private:
      REGISTER_FRAME_DECLARATION(Frame);

    protected:

      static uint8_t RESIZING_RIGHT;
      static uint8_t RESIZING_LEFT;
      static uint8_t RESIZING_TOP;
      static uint8_t RESIZING_BOTTOM;
      static int RESIZING_MARGIN;

      bool m_resizing;
      uint8_t m_resizableFlags;
      uint8_t m_resizingFlags;
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

      virtual void SetPosition(const Vec2f& pos) override;

      // Check if the mouse is within the resize window
      bool IsHoverResize(const Vec2f& mousePos);
      void SetResizeFlags(const Vec2f& mousePos);

      // Resize the window the mouse position
      void Resize(const Vec2f& mousePos);

      // Clamp the container inside window after resizing it.
      void ResizeScreenClamp();

      void OnEvent(Event& event, const Vec2f& componentPos) override;

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

      void SetStylingFile(const MetaFile& metaFile);
  };
}
