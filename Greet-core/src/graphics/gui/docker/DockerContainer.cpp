#include "DockerContainer.h"

#include <graphics/fonts/FontManager.h>
#include <graphics/gui/Docker.h>
#include <graphics/gui/docker/DockerTab.h>
#include <input/Input.h>

#include <sstream>

namespace Greet
{
  DockerContainer::DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, activeTab{0}
  {
    weight = GUIUtils::GetFloatFromXML(object, "weight", 1.0);
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerTab")
      {
        AddTab(children.size(), new DockerTab(objectChild, docker, this));
      }
      else
      {
        Log::Error("Unknown component in DockerContainer. Component=", objectChild.GetName());
      }
    }
    if(children.size() > 0)
      children[0]->ShowTab();
  }

  DockerContainer::DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, activeTab{0}
  {
    AddTab(0, tab);
  }

  DockerContainer::~DockerContainer()
  {
    for(auto&& child : children)
      delete child;
  }

  void DockerContainer::Render(GUIRenderer* renderer) const
  {
    if(activeTab < 0 || activeTab >= children.size())
      return;

    renderer->PushViewport(position, size);
    children[activeTab]->Render(renderer);

    renderer->PushTranslation(docker->GetPosition() + position);
    Button* tabButton = docker->GetTabButton();
    for(int i = 0;i<children.size();i++)
    {
      if(i == activeTab)
        tabButton->SetCurrentStyle("active");
      else if(hover && i == hoverTab)
        tabButton->SetCurrentStyle("hover");
      else
        tabButton->SetCurrentStyle("normal");
      tabButton->SetText(children[i]->GetTitle());
      tabButton->Measure({0, 0}, {0, 0});
      tabButton->PreRender(renderer, {(float)i * (float)tabButton->GetWidth(), 0.0f});
      tabButton->Render(renderer);
      tabButton->PostRender(renderer);
    }

    Component* splitIcon = docker->GetSplitIcon();
    if(docker->IsHoldingTab() && Utils::IsInside(Input::GetMousePosPixel(), position + docker->GetRealPosition() + docker->GetTotalPadding(), size))
    {
      splitIcon->PreRender(renderer, GetTopSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetBottomSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetLeftSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetRightSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
    }
    renderer->PopTranslation();
    renderer->PopViewport();
  }

  void DockerContainer::Update(float timeElapsed)
  {
    for(auto&& child : children)
    {
      child->Update(timeElapsed);
    }
  }

  void DockerContainer::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(activeTab < 0 || activeTab >= children.size())
      return;
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1)
      {
        Vec2f pos = e.GetPosition() - componentPos;
        int tab = GetTab(pos);
        if(tab < children.size())
        {
          SelectTab(tab);
          docker->GrabDockerTab(children[tab]);
          return;
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      Vec2f pos = e.GetPosition() - componentPos;
      int tab = GetTab(pos);
      if(tab < children.size())
      {
        HoverTab(tab);
        return;
      }
      else
      {
        UnhoverTab();
      }
    }
    children[activeTab]->OnEvent(event, componentPos + GetTabOffset());
  }

  void DockerContainer::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2f& componentPos)
  {
    Vec2f mousePos = event.GetPosition() - position - componentPos;
    DockerSplit* split = static_cast<DockerSplit*>(parent);
    int index = split->GetDockerIndex(this);
    bool splitContainer = false;
    bool vertical = false;
    bool before = false;
    if(Utils::IsInside(mousePos, GetTopSplitPos(), GetSplitSize()))
    {
      AddSplitAbove(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetBottomSplitPos(), GetSplitSize()))
    {
      AddSplitBelow(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetLeftSplitPos(), GetSplitSize()))
    {
      AddSplitLeft(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetRightSplitPos(), GetSplitSize()))
    {
      AddSplitRight(tab);
      return;
    }
    // Only move inside the current container
    if(tab->GetContainer() == this)
    {
      int tabIndex = GetTab(event.GetPosition() - position - componentPos);
      if(tabIndex < children.size())
      {
        int current = GetTabIndex(tab);
        if(current == tabIndex)
          return;
        if(current > tabIndex)
          std::rotate(children.begin()+tabIndex, children.begin() + current, children.begin() + current+1);
        else
          std::rotate(children.begin()+current, children.begin() + 1, children.begin() + tabIndex+1);
        UnhoverTab();
        SelectTab(tabIndex);
        return;
      }
      return;
    }
    else
    {
      DockerContainer* tabContainer = tab->GetContainer();

      int tabIndex = GetTab(event.GetPosition() - componentPos);
      int tabIndexContainer = tabContainer->GetTabIndex(tab);
      AddTab(tabIndex, tab);

      // Fix this container
      tabContainer->RemoveTab(tab);
      SelectTab(tabIndex);
      return;
    }
  }

  void DockerContainer::SelectTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound: ", i, " >= ", children.size());
    if(i != activeTab)
    {
      if(activeTab >= 0 && activeTab < children.size())
        children[activeTab]->HideTab();
      children[i]->ShowTab();
    }
    activeTab = i;
  }

  void DockerContainer::ClampSelectedTab()
  {
    if(activeTab >= children.size())
    {
      SelectTab(children.size() - 1);
    }
  }

  void DockerContainer::HoverTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound: ", i, " >= ", children.size());
    hoverTab = i;
    hover = true;
  }

  void DockerContainer::UnhoverTab()
  {
    hover = false;
  }

  void DockerContainer::AddTab(int i, DockerTab* tab)
  {
    ASSERT(i >= 0 && i <= children.size(), "Index out of bound: ", i, " >= ", children.size());
    children.insert(children.begin() + i, tab);
    tab->SetPosition(position + GetTabOffset());
    tab->SetSize(size - GetTabOffset(), true);
    tab->docker = docker;
    tab->SetContainer(this);
  }

  void DockerContainer::RemoveTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound: ", i, " >= ", children.size());
    children.erase(children.begin() + i);

    DockerSplit* split = static_cast<DockerSplit*>(parent);
    if(children.size() == 0)
      docker->MarkDirty();
    else
      ClampSelectedTab();
  }


  void DockerContainer::RemoveTab(DockerTab* tab)
  {
    RemoveTab(GetTabIndex(tab));
  }

  Component* DockerContainer::GetComponentByNameNoCast(const std::string& name)
  {
    for(auto&& child : children)
    {
      Component* comp = child->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  Vec2f DockerContainer::GetMinSize() const
  {
    return Vec2f{20, docker->GetTabButton()->GetHeight()+20};
  }

  int DockerContainer::GetTabIndex(DockerTab* tab)
  {
    for(int i = 0;i<children.size(); ++i)
    {
      if(children[i] == tab)
        return i;
    }
    ASSERT(false, "Tab is not inside DockerContainer");
    return children.size();
  }

  int DockerContainer::GetTab(const Vec2f& mousePos)
  {
    if(mousePos.y >= 0 && mousePos.y < docker->GetTabButton()->GetHeight())
    {
      int tab = floor(mousePos.x / (docker->GetTabButton()->GetWidth() + 1));
      if(tab < children.size())
      {
        return tab;
      }
    }
    return children.size();
  }

  int DockerContainer::GetTabCount()
  {
    return children.size();
  }

  void DockerContainer::SetGUIScene(GUIScene* scene)
  {
    for(auto&& child : children)
    {
      child->SetGUIScene(scene);
    }
  }

  void DockerContainer::SetPosition(const Vec2f& _position)
  {
    position = _position;
    for(auto&& child : children)
    {
      child->SetPosition(_position + GetTabOffset());
    }
  }

  void DockerContainer::SetSize(const Vec2f& avSize, bool abRemeasure)
  {
    size = avSize;
    for(auto&& child : children)
    {
      child->SetSize(avSize - GetTabOffset(), abRemeasure);
    }
  }

  DockerTab* DockerContainer::GetTab(const std::string& tabName) const
  {
    for(auto&& child : children)
    {
      if(child->GetTitle() == tabName)
        return child;
    }
    return nullptr;
  }

  void DockerContainer::LoadFrameStyle(const MetaFile& metaFile)
  {
    for(auto&& child : children)
    {
      child->LoadFrameStyle(metaFile);
    }
  }

  const Vec2f& DockerContainer::GetTopSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2f center = size*0.5;
    const Vec2f& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2f pos;
    pos = center - dockerSize*0.5 - Vec2f{0, dockerSize.h + spacing};
    return pos;
  }

  const Vec2f& DockerContainer::GetBottomSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2f center = size*0.5;
    const Vec2f& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2f pos;
    pos = center - dockerSize*0.5 + Vec2f{0, dockerSize.h + spacing};
    return pos;
  }

  const Vec2f& DockerContainer::GetLeftSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2f center = size*0.5;
    const Vec2f& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2f pos;
    pos = center - dockerSize*0.5 - Vec2f{dockerSize.w + spacing, 0};
    return pos;
  }

  const Vec2f& DockerContainer::GetRightSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2f center = size*0.5;
    const Vec2f& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2f pos;
    pos = center - dockerSize*0.5 + Vec2f{dockerSize.w + spacing, 0};
    return pos;
  }

  const Vec2f& DockerContainer::GetSplitSize() const
  {
    // TODO: Store all this in a variable
    static Vec2f splitSize = {30,30};
    return splitSize;
  }

  const Vec2f& DockerContainer::GetTabOffset() const
  {
    static Vec2f offset;
    offset = {0, docker->GetTabButton()->GetHeight()};
    return offset;
  }

  void DockerContainer::AddSplitLeft(DockerTab* tab)
  {
    AddSplit(tab, false, true);
  }

  void DockerContainer::AddSplitRight(DockerTab* tab)
  {
    AddSplit(tab, false, false);
  }

  void DockerContainer::AddSplitAbove(DockerTab* tab)
  {
    AddSplit(tab, true, true);
  }

  void DockerContainer::AddSplitBelow(DockerTab* tab)
  {
    AddSplit(tab, true, false);
  }

  void DockerContainer::AddSplit(DockerTab* tab, bool vertical, bool before)
  {
    float oldWeight = weight;

    DockerSplit* split = static_cast<DockerSplit*>(parent);
    int index = split->GetDockerIndex(this);

    if(tab->GetContainer() == this && children.size() == 1)
    {
      return;
    }
    if(tab->GetContainer() != nullptr)
      tab->GetContainer()->RemoveTab(tab);

    DockerContainer* container = new DockerContainer(tab, docker, split);
    DockerSplit* newSplit;
    if(before)
      newSplit = new DockerSplit({container, this}, docker, split, vertical);
    else
      newSplit = new DockerSplit({this, container}, docker, split, vertical);

    split->ReplaceDocker(index, newSplit);
    newSplit->SetPosition(position);
    newSplit->SetSize(size, true);
    newSplit->SetWeight(oldWeight);
    container->SetWeight(1);
    SetWeight(1);
  }

  void DockerContainer::DebugPrint(int indent)
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
}
