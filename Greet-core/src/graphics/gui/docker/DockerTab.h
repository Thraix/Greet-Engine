#pragma once

#include <graphics/gui/Component.h>
#include <utils/xml/XMLObject.h>

namespace Greet {

  class Docker;
  class DockerContainer;


  class DockerTab
  {
    public:
      friend class Docker;
      friend class DockerContainer;
      friend class DockerSplit;
      typedef std::function<void(Docker* c, DockerTab* tab)> OnTabChangeCallback;

    protected:
      OnTabChangeCallback onTabShowCallback;
      OnTabChangeCallback onTabHideCallback;
      Docker* docker;
      DockerContainer* parentContainer;

      std::string title;
      Component* component;
      bool shown = false;
      bool remeasure = false;
      Vec2f size;

    public:
      DockerTab(const std::string& title, Component* component);
      DockerTab(const XMLObject& object, Docker* docker, DockerContainer* parent);
      virtual ~DockerTab();

      const std::string& GetTitle() const;
    public:
      void Render(GUIRenderer* renderer) const;
      void Update(float timeElapsed);
      void OnEvent(Event& event, const Vec2f& componentPos);

      Component* GetComponentByNameNoCast(const std::string& name);
      DockerContainer* GetContainer() const;
      void LoadFrameStyle(const MetaFile& metaFile);

      void SetOnTabShowCallback(OnTabChangeCallback callback);
      void SetOnTabHideCallback(OnTabChangeCallback callback);

      void SetContainer(DockerContainer* parentContainer);
      void SetGUIScene(GUIScene* scene);
      void SetPosition(const Vec2f& position);
      void SetSize(const Vec2f& avSize, bool abRemeasure);

      Docker* GetDocker() const;
      void SetDocker(Docker* docker);

    protected:
      void ShowTab();
      void HideTab();
  };
}
