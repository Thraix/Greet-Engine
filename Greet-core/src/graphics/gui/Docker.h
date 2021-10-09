#pragma once

#include <graphics/gui/Button.h>
#include <graphics/gui/Component.h>
#include <graphics/gui/docker/DockerSplit.h>
#include <utils/xml/XMLObject.h>

namespace Greet {

  class Docker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Docker);
      friend class DockerSplit;
      friend class DockerContainer;
      friend class DockerTab;

    protected:
      DockerSplit* split;
      DockerTab* grabbedTab;
      float grabbedDistance = 0;
      bool childrenChanged = false;

      Button* tabButton;
      Component* splitIcon;
    private:
      //////////////////
      // Style variables
      Color edgeColor = Color{1, 1, 1};
      float edgeWidth = 10;
      float edgeBorderSize = 0;
      Color edgeBorderColor = Color{1, 1, 1};

    public:
      Docker(const XMLObject& object, Component* parent);
      virtual ~Docker();

      void GrabDockerTab(DockerTab* tab);
      void HandleDroppedTab(MouseReleaseEvent& event, const Vec2f& componentPos);

      Button* GetTabButton() const;
      Component* GetSplitIcon() const;

      // Overrided functions
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2f& componentPos) override;
      void OnMeasured() override;

      virtual void LoadFrameStyle(const MetaFile& metaFile) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      void SetGUIScene(GUIScene* scene) override;

      bool IsHoldingTab() const;

      void MarkDirty();

      inline void AddSplitLeft(DockerTab* tab);
      inline void AddSplitRight(DockerTab* tab);
      inline void AddSplitAbove(DockerTab* tab);
      inline void AddSplitBelow(DockerTab* tab);

      DockerTab* GetTab(const std::string& tabName) const;

    private:
      void Split(DockerTab* tab, bool vertical, bool before);
  };
}
