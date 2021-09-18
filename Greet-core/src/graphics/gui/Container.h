#pragma once

#include <graphics/gui/Component.h>
#include <utils/xml/XMLObject.h>

#include <vector>

namespace Greet {
  class Container : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Container);
    protected:
      std::vector<Component*> m_components;

      //////////////////
      // Style variables
      bool vertical = true;
      float spacing = 10;

    public:
      Container();
      Container(const std::string& name, Component* parent);
      Container(const XMLObject& object, Component* parent);


      virtual ~Container();

      void Measure(const Vec2f& emptyParentSpace, const Vec2f& percentageFill) override;
      Vec2f GetMeasureFillSize() override;
      float GetMeasureTotalWeight() override;

      virtual void PostConstruction() override;

      virtual void RenderHandle(GUIRenderer* renderer) const override;
      virtual void UpdateHandle(float timeElapsed) override;

      void AddComponent(Component* component);
      Component* RemoveComponent(uint32_t index);
      Component* RemoveComponent(Component* component);
      Component* GetComponent(uint32_t index);
      size_t GetComponentCount() const;

      virtual void LoadFrameStyle(const MetaFile& metaFile) override;

      bool IsVertical() const;
      Container& SetVertical(bool v);
      Container& SetSpacing(float spacing);

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;

      void OnMousePressEventHandler(MousePressEvent& event, const Vec2f& componentPos) override;
      void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2f& componentPos) override;

      virtual void SetGUIScene(GUIScene* scene) override;

      virtual Component* GetComponentByNameNoCast(const std::string& name) override;

    private:
      void MeasureChildren(const Vec2f& weightTotals);
  };
}
