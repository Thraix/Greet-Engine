#pragma once

#include <graphics/gui/docker/DockerTab.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XMLObject.h>

#include <vector>

namespace Greet {

  class Docker;
  class DockerSplit;

  class DockerContainer : public DockerInterface
  {
    protected:
      std::vector<DockerTab*> children;
      uint32_t activeTab = 0;

      bool hover = false;
      uint32_t hoverTab = 0;

    public:
      DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parentSplit);
      DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parentSplit);
      virtual ~DockerContainer();

      void SelectTab(int index);
      void ClampSelectedTab();
      void HoverTab(int index);
      void UnhoverTab();
      void AddTab(int index, DockerTab* tab);
      void RemoveTab(int index);
      void RemoveTab(DockerTab* tab);

      int GetTabIndex(DockerTab* tab);

      int GetTab(const Vec2f& mousePos);
      int GetTabCount();

    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2f& componentPos) override;

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2f& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2f GetMinSize() const override;

      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2f& position) override;
      void SetSize(const Vec2f& avSize, bool abRemeasure) override;

      DockerTab* GetTab(const std::string& tabName) const override;

      void LoadFrameStyle(const MetaFile& metaFile) override;

      inline void AddSplitLeft(DockerTab* tab);
      inline void AddSplitRight(DockerTab* tab);
      inline void AddSplitAbove(DockerTab* tab);
      inline void AddSplitBelow(DockerTab* tab);

    private:
      const Vec2f& GetTopSplitPos() const;
      const Vec2f& GetBottomSplitPos() const;
      const Vec2f& GetLeftSplitPos() const;
      const Vec2f& GetRightSplitPos() const;
      const Vec2f& GetSplitSize() const;

      void AddSplit(DockerTab* tab, bool vertical, bool before);

      const Vec2f& GetTabOffset() const;

      void DebugPrint(int indent) override
      {
        std::stringstream ss;
        for(int i = 0;i<indent;i++)
        {
          ss << "-";
        }
        if(children.size() == 0)
          ss << "No children";
        else
          ss << children[0]->GetTitle();
        ss << " w=" << weight;
        Log::Info(ss.str());
      }
  };
}
