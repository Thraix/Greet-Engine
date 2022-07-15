#include "DockerTab.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerTab::DockerTab(const std::string& title, Component* component)
    : title{title}, docker{nullptr}, parentContainer{nullptr}, component{component}
  {
  }

  DockerTab::DockerTab(const XMLObject& object, Docker* docker, DockerContainer* parentContainer)
    : docker{docker}, parentContainer{parentContainer}, component{nullptr}
  {
    title = GUIUtils::GetStringFromXML(object, "title", "");
    if(object.GetObjectCount() == 0)
    {
      Log::Warning("DockerTab contains no component");
      component = new Component({"Component", {}, ""}, docker);
    }
    else
    {
      component = ComponentFactory::GetComponent(object.GetObject(0), docker);
      if(object.GetObjectCount() >= 2)
      {
        Log::Warning("DockerTab contains more than one component. Only one will display. Put them in a Container");
      }
      component->PostConstruction();
    }
  }

  DockerTab::~DockerTab()
  {
    if(component->ownedByGUISystem)
      delete component;
  }


  void DockerTab::Render(GUIRenderer* renderer) const
  {
    component->PreRender(renderer, docker->GetPosition());
    component->RenderHandle(renderer);
    component->PostRender(renderer);
  }

  void DockerTab::Update(float timeElapsed)
  {
    if(remeasure)
    {
      remeasure = false;
      component->Measure(size, {1, 1});
      component->OnMeasured();
    }
    component->UpdateHandle(timeElapsed);
  }

  void DockerTab::OnEvent(Event& event, const Vec2f& componentPos)
  {
    component->OnEventHandler(event, componentPos + Vec2f{0.0f, 0.0f});
  }

  Component* DockerTab::GetComponentByNameNoCast(const std::string& name)
  {
    return component->GetComponentByNameNoCast(name);
  }

  const std::string& DockerTab::GetTitle() const
  {
    return title;
  }

  DockerContainer* DockerTab::GetContainer() const
  {
    return parentContainer;
  }

  void DockerTab::LoadFrameStyle(const MetaFile& metaFile)
  {
    component->LoadFrameStyle(metaFile);
  }

  void DockerTab::SetContainer(DockerContainer* _parentContainer)
  {
    parentContainer = _parentContainer;
  }

  void DockerTab::SetGUIScene(GUIScene* scene)
  {
    component->SetGUIScene(scene);
  }

  void DockerTab::SetPosition(const Vec2f& _position)
  {
    component->SetPosition(_position);
  }

  void DockerTab::SetSize(const Vec2f& avSize, bool abRemeasure)
  {
    size = avSize;
    if(abRemeasure)
    {
      component->Measure(size, {1, 1});
      component->OnMeasured();
    }
    else
      remeasure = true;
  }

  void DockerTab::SetOnTabShowCallback(OnTabChangeCallback callback)
  {
    onTabShowCallback = callback;
    // Call the callback if the tab is already shown
    // This is done to solve problems with the callback not being called
    // for the initial setup
    if(shown)
      onTabShowCallback(docker, this);
  }

  void DockerTab::SetOnTabHideCallback(OnTabChangeCallback callback)
  {
    onTabHideCallback = callback;
    // Do not call the callback as with the SetOnTabShowCallback
    // As this doesn't have the same problem with initialization
  }

  void DockerTab::ShowTab()
  {
    if(!shown && onTabShowCallback)
      onTabShowCallback(docker, this);
    shown = true;
  }

  void DockerTab::HideTab()
  {
    if(shown && onTabHideCallback)
      onTabHideCallback(docker, this);
    shown = false;
  }
}
