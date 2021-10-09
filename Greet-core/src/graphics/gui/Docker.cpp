#include "Docker.h"

#include <input/InputDefines.h>
#include <graphics/gui/docker/DockerTab.h>
#include <graphics/gui/docker/DockerContainer.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Docker);

  Docker::Docker(const XMLObject& object, Component* parent)
    : Component(object, parent), split{nullptr}, grabbedTab{nullptr}, tabButton{nullptr}, splitIcon{nullptr}
  {
    AddStyleVariables(StylingVariables{
      .colors = {{"edgeColor", &edgeColor}, {"edgeBorderColor", &edgeBorderColor}},
      .floats = {{"edgeWidth", &edgeWidth}, {"edgeBorder", &edgeBorderSize}}
    });
    LoadStyles(object);

    splitIcon = new Component({"StyleIcon", {}, ""}, nullptr);
    tabButton = new Button({"TabButton", {}, ""}, nullptr);
    for(auto&& child : object.GetObjects())
    {
      if(child.GetName() == "DockerSplit")
      {
        if(split == nullptr)
          split = new DockerSplit(child, this, nullptr);
        else
          Log::Error("NOTIMPLEMENTED: Docker currently only supports one DockerSplit");
      }
      else if(child.GetName() == "DockerContainer")
      {
        Log::Error("NOTIMPLEMENTED: Docker containing only DockerContainer is currently not supported");
        // This should contain DockerTabs which are actual tabs
        // Maybe this should be wrapped in a DockerSplit
        // TODO: Add docker container component
      }
      else if(child.GetName() == "DockerTab")
      {
        Log::Error("NOTIMPLEMENTED: Docker containing only DockerTab is currently not supported");
        // Add Docker tab component
        // TODO: Maybe this should be wrapped in a docker container
      }
      else if(child.GetName() == "TabButton")
      {
        delete tabButton;
        tabButton = new Button(child, nullptr);
      }
      else if(child.GetName() == "SplitIcon")
      {
        delete splitIcon;
        splitIcon = new Component(child, nullptr);
        splitIcon->Measure({0, 0}, {0, 0});
      }
      else
      {
        Log::Error("Unknown component in Docker. Component=", child.GetName());
      }
    }
    if(split == nullptr)
    {
      Log::Error("No DockerSplit found in Docker");
      split = new DockerSplit({"DockerSplit",{},""}, this, nullptr);
    }

    tabButton->AddStyle("active", "normal");
    tabButton->Measure({0, 0}, {1, 1});
    splitIcon->Measure({0, 0}, {1, 1});
  }

  Docker::~Docker()
  {
    delete splitIcon;
    delete tabButton;
    delete split;
  }

  void Docker::GrabDockerTab(DockerTab* tab)
  {
    grabbedDistance = 0;
    grabbedTab = tab;
  }

  Button* Docker::GetTabButton() const
  {
    return tabButton;
  }

  Component* Docker::GetSplitIcon() const
  {
    return splitIcon;
  }

  void Docker::HandleDroppedTab(MouseReleaseEvent& event, const Vec2f& componentPos)
  {
    Vec2f mousePos = event.GetPosition() - componentPos;

    int splitLimit = 0;

    if(mousePos.x < splitLimit)
    {
      AddSplitLeft(grabbedTab);
    }
    else if(mousePos.x >= GetContentSize().w - splitLimit)
    {
      AddSplitRight(grabbedTab);
    }
    else if(mousePos.y < splitLimit)
    {
      AddSplitAbove(grabbedTab);
    }
    else if(mousePos.y >= GetContentSize().h - splitLimit)
    {
      AddSplitBelow(grabbedTab);
    }
    else if(IsMouseInside(mousePos))
    {
      split->HandleDroppedTab(grabbedTab, event, componentPos);
    }
    grabbedTab = nullptr;
  }

  void Docker::Render(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(GetTotalPadding());
    split->Render(renderer);
    renderer->PopTranslation();
  }

  void Docker::Update(float timeElapsed)
  {
    if(childrenChanged)
    {
      split->MergeSimilarSplits();
      childrenChanged = false;
    }
    split->Update(timeElapsed);
  }

  void Docker::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if(IsHoldingTab())
      {
        if(e.GetButton() == GREET_MOUSE_1)
        {
          HandleDroppedTab(e, componentPos + GetTotalPadding());
        }
        return;
      }
      else if(grabbedTab != nullptr)
      {
        grabbedTab = nullptr;
        return;
      }
      split->OnEvent(event, componentPos + GetTotalPadding());
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(grabbedTab != nullptr)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        grabbedDistance += e.GetDeltaPosition().Length();
      }
    }
    split->OnEvent(event, componentPos + GetTotalPadding());
  }

  void Docker::OnMeasured()
  {
    split->SetSize(GetContentSize(), true);
    // Update positions of child dockers
    split->SetPosition(Vec2f{0.0f, 0.0f});
  }

  void Docker::LoadFrameStyle(const MetaFile& metaFile)
  {
    Component::LoadFrameStyle(metaFile);
    split->LoadFrameStyle(metaFile);
    tabButton->LoadFrameStyle(metaFile);
    tabButton->Measure({0, 0}, {1, 1});
    splitIcon->LoadFrameStyle(metaFile);
    splitIcon->Measure({0, 0}, {1, 1});
  }

  Component* Docker::GetComponentByNameNoCast(const std::string& name)
  {
    Component* comp = Component::GetComponentByNameNoCast(name);
    if(comp)
      return comp;
    return split->GetComponentByNameNoCast(name);
  }

  void Docker::SetGUIScene(GUIScene* scene)
  {
    guiScene = scene;
    split->SetGUIScene(scene);
  }

  bool Docker::IsHoldingTab() const
  {
    return grabbedTab != nullptr && grabbedDistance > 20;
  }

  DockerTab* Docker::GetTab(const std::string& tabName) const
  {
    return split->GetTab(tabName);
  }

  void Docker::MarkDirty()
  {
    childrenChanged = true;
  }

  void Docker::AddSplitLeft(DockerTab* tab)
  {
    Split(tab, false, true);
  }

  void Docker::AddSplitRight(DockerTab* tab)
  {
    Split(tab, false, false);
  }

  void Docker::AddSplitAbove(DockerTab* tab)
  {
    Split(tab, true, true);
  }

  void Docker::AddSplitBelow(DockerTab* tab)
  {
    Split(tab, true, false);
  }

  void Docker::Split(DockerTab* tab, bool vertical, bool before)
  {
    if(tab->GetContainer() != nullptr)
      tab->GetContainer()->RemoveTab(tab);
    tab->docker = this;

    if(split->IsVertical() != vertical)
    {
      split = new DockerSplit(split, this, nullptr, vertical);
    }

    DockerContainer* container = new DockerContainer(tab, this, nullptr);
    split->AddDocker(before ? 0 : split->GetDockerCount(), container);
    OnMeasured();
  }
}
