#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Text.h>

namespace Greet
{
  class TextBox : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(TextBox);
    public:
      typedef std::function<void(Component*, const std::string& before, const std::string& after)> OnTextChangedCallback;
    protected:
      //////////////////
      // Style variables
      Text text;
      Text hintText;

      int cursorPos;
      int selectionPos;
      float textOffset;
      float cursorBlinkTimer;
      bool ctrlDown;
      bool shiftDown;

      bool password;
      std::string str;

      OnTextChangedCallback onTextChangedCallback;


    public:
      TextBox(const std::string& name, Component* parent);
      TextBox(const XMLObject& object, Component* parent);

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;

      const std::string& GetText() const;
      TextBox& SetText(const std::string& text);

      TextBox& SetFont(const std::string& fontName);
      TextBox& SetFontSize(float fontSize);
      TextBox& SetHintFont(const std::string& fontName);
      TextBox& SetHintFontSize(float fontSize);

      void OnEvent(Event& event, const Vec2f& componentPos) override;
      void KeyPress(KeyPressEvent& event);

      void SetOnTextChangedCallback(OnTextChangedCallback callback);

    protected:
      void RemoveText(uint32_t start, uint32_t n);
      void RecenterText();
      void MoveCursor(int delta);
      void MoveCursorWord(bool forward);
      int GetCursorPos() const;
      int GetSelectionPos() const;

      void CallOnTextChangedCallback(const std::string& before, const std::string& after);
  };

}
