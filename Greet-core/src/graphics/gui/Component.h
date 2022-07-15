#pragma once

#include <graphics/gui/ComponentFactory.h>
#include <graphics/gui/GUIScene.h>
#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <utils/MetaFile.h>

#include <unordered_map>

namespace Greet
{
  class Component
  {
    public:
      friend class GUIScene;
    private:
      REGISTER_COMPONENT_DECLARATION(Component);
    public:

      // Typedef callbacks
      typedef std::function<void(Component* c)> OnClickCallback;
      typedef std::function<void(Component* c)> OnPressCallback;
      typedef std::function<void(Component* c)> OnReleaseCallback;

    private:
      StylingVariables styleVariables;

    public:
      bool ownedByGUISystem = true; // If false the GUI system will not free the memory of this component

    protected:
      std::string name;
      std::string componentType;

      //////////////////
      // Styles
      std::unordered_map<std::string, ComponentStyle> styles;
      std::string currentStyle = "";

      //////////////////
      // Style variables
      Color backgroundColor{0};
      float backgroundRadius = 0;
      int backgroundRoundedPrecision = 3;
      Color borderColor = {0};
      float borderRadius = 0;
      int borderRoundedPrecision = 3;
      TLBR border = {0, 0, 0, 0};
      TLBR margin = {0, 0, 0, 0};
      TLBR padding = {0, 0, 0, 0};
      GUISize width = GUISize{0, 1, GUISize::Type::Wrap};
      GUISize height = GUISize{0, 1, GUISize::Type::Wrap};

    protected:
      GUIScene* guiScene = nullptr;
      Component* parent = nullptr;
      bool remeasure = true;

      Vec2f pos;

      bool isFocused;
      bool isHovered;
      bool pressed;

      // Callbacks
      OnClickCallback onClickCallback;
      OnPressCallback onPressCallback;
      OnReleaseCallback onReleaseCallback;

    public:
      Component(const std::string& name, Component* parent, const std::string& componentType);
      Component(const XMLObject& object, Component* parent);
      virtual ~Component() = default;

      virtual void PostConstruction(){}

      virtual void Measure(const Vec2f& emptyParentSpace, const Vec2f& percentageFill);

      void Remeasure();

      // Size has updated
      virtual void OnMeasured(){}

      virtual Vec2f GetMeasureFillSize();
      virtual float GetMeasureTotalWeight();

      // Push translation to renderer
      virtual void PreRender(GUIRenderer* renderer, const Vec2f& translation) const;

      // Render component
      virtual void RenderHandle(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const{}

      // Pop translation from renderer
      virtual void PostRender(GUIRenderer* renderer) const;

      virtual void UpdateHandle(float timeElapsed);
      virtual void Update(float timeElapsed){}

      // These methods generally handle internal stuff and shouldn't be overriden if
      // not necessary use their non EventHandler counterparts instead.
      virtual void OnEventHandler(Event& event, const Vec2f& componentPos);
      virtual void OnMousePressEventHandler(MousePressEvent& event, const Vec2f& componentPos);
      virtual void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2f& componentPos);
      virtual void OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos);

      virtual void OnEvent(Event& event, const Vec2f& componentPos) {}


      // Callbacks
      void SetOnClickCallback(OnClickCallback callback);
      void SetOnPressCallback(OnPressCallback callback);
      void SetOnReleaseCallback(OnReleaseCallback callback);

      Vec2f GetPosition() const;
      Vec2f GetRealPosition() const;
      Vec2f GetSize() const;
      Component* GetParent() const;

      virtual float GetWrapWidth() const { return 100; };
      virtual float GetWrapHeight() const { return 100; };

      // Will return the stored value
      virtual float GetWidth() const;
      virtual float GetHeight() const;
      GUISize::Type GetWidthType() const;
      GUISize::Type GetHeightType() const;

      // Value of the sizeType, 1w will return 1 and not the size of the component
      Vec2f GetSizeValue() const;

      // Sets the sizeType size and not the actuall component size.
      Component& SetWidth(float width);
      Component& SetHeight(float height);
      Component& SetWidthType(GUISize::Type width);
      Component& SetHeightType(GUISize::Type height);

      // Faster to use since it sets all values and does a single remeasure.
      Component& SetSize(float width, float height, GUISize::Type widthType, GUISize::Type heightType);

      ComponentStyle& GetStyle(const std::string& stylename);
      const ComponentStyle& GetStyle(const std::string& stylename) const;
      Component& AddStyle(const std::string& stylename, const std::string& inherit = "");
      Component& AddStyleVariables(const StylingVariables& variables);
      Component& SetStyleVariables(const StylingVariables& variables);
      void LoadStyles(const XMLObject& xmlObject);
      virtual void LoadFrameStyle(const MetaFile& metaFile);
      Component& LoadStyle(const std::string& stylename, const Styling& styling);
      void SetCurrentStyle(const std::string& stylename);

      virtual bool IsMouseInside(const Vec2f& parentMouse) const;
      virtual void SetPosition(const Vec2f& pos);
      void SetSize(const Vec2f& size);

      virtual void SetGUIScene(GUIScene* scene);

      // These four will only be called if the component is focusable
      virtual void OnFocused();
      virtual void OnUnfocused();
      virtual void Focused(){}
      virtual void Unfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}
      bool IsFocused() const { return isFocused; }

      virtual void ChildChangedFocus(bool focused) {}

      virtual bool UsingMouse();

      Component* GetRootNode();
      Vec2f GetTotalPadding() const;
      const TLBR& GetMargin() const;
      const TLBR& GetPadding() const;
      const TLBR& GetBorder() const;
      Vec2f GetContentSize() const;
      const std::string& GetName() const;

      uint32_t GetComponentDepth() const;

      template <typename T>
      T* GetComponentByName(const std::string& name)
      {
        T* component = dynamic_cast<T*>(GetComponentByNameNoCast(name));
        if(component == nullptr)
          Log::Warning("Could not find Component with given type (name = %s)", name);
        return component;
      }

      virtual Component* GetComponentByNameNoCast(const std::string& name);

      friend std::ostream& operator<<(std::ostream& stream, const Component& component)
      {
        return stream << component.GetName();
      }

    protected:
      virtual void CallOnClickCallback();
      virtual void CallOnPressCallback();
      virtual void CallOnReleaseCallback();
  };
}
