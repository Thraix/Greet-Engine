#include "DockerSplit.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerSplit::DockerSplit(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}
  {
    vertical = GUIUtils::GetBooleanFromXML(object, "vertical", false);
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerSplit")
      {
        children.push_back(new DockerSplit(objectChild, docker, this));
      }
      else if(objectChild.GetName() == "DockerContainer")
      {
        children.push_back(new DockerContainer(objectChild, docker, this));
      }
      else
      {
        Log::Error("Unknown component in DockerSplit. Component=", objectChild.GetName());
      }
    }
  }

  DockerSplit::DockerSplit(DockerSplit* split, Docker* docker, DockerSplit* parent, bool vertical)
    : DockerInterface{docker, parent}, children{split}, vertical{vertical}
  {
    split->SetParent(this);
  }

  DockerSplit::~DockerSplit()
  {
    for(auto&& child : children)
      delete child;
  }

  void DockerSplit::Render(GUIRenderer* renderer) const
  {
    for(auto&& child : children)
    {
      child->Render(renderer);
    }
  }

  void DockerSplit::Update(float timeElapsed)
  {
    for(auto&& child : children)
    {
      child->Update(timeElapsed);
    }
  }

  void DockerSplit::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      Vec2 mousePos = e.GetPosition() - componentPos;
      if((vertical && mousePos.x >= 0 && mousePos.x < size.w) || (!vertical && mousePos.y >= 0 && mousePos.y < size.h))
      {
        int vecIndex = vertical ? 1 : 0;
        for(int i = 0;i<children.size()-1;i++)
        {
          mousePos -= children[i]->GetSize();
          if(mousePos[vecIndex] >= -5 && mousePos[vecIndex] < 5)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
            return;
          }
        }
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        if(child->IsMouseInside(e.GetPosition() - pos))
          child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex];
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(grabbingEdge)
      {
        Vec2 mousePos = e.GetPosition() - componentPos;

        DockerInterface* dockerThis = children[grabbedEdgeIndex];
        DockerInterface* dockerNext = children[grabbedEdgeIndex+1];

        Vec2 sizeThis = dockerThis->GetSize();
        Vec2 posNext = dockerNext->GetPosition();
        Vec2 sizeNext = dockerNext->GetSize();

        int vecIndex = vertical ? 1 : 0;

        float offset = e.GetDeltaPosition()[vecIndex];

        sizeThis[vecIndex] += offset;
        posNext[vecIndex] += offset;
        sizeNext[vecIndex] -= offset;

        dockerThis->SetSize(sizeThis);
        dockerNext->SetPosition(posNext);
        dockerNext->SetSize(sizeNext);
        return;
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        if(child->IsMouseInside(e.GetPosition() - pos))
          child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex];
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      if(grabbingEdge)
      {
        grabbingEdge = false;
        return;
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      for(auto&& child : children)
      {
        if(child->IsMouseInside(e.GetPosition() - pos))
          child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex];
      }
      return;
    }

    Vec2 pos = componentPos;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->OnEvent(event, pos);
      pos[vecIndex] += child->GetSize()[vecIndex];
    }
  }

  bool DockerSplit::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos)
  {
    Vec2 mousePos = event.GetPosition() - componentPos;
    Vec2 position = {0,0};
    for(auto&& child : children)
    {
      if(mousePos.x >= position.x && mousePos.y >= position.y && mousePos.x < position.x + child->GetSize().x && mousePos.y < position.y + child->GetSize().y)
      {
        return child->HandleDroppedTab(tab, event, componentPos + position);
      }
      int vecIndex = vertical ? 1 : 0;
      position[vecIndex] += child->GetSize()[vecIndex];
    }
    return false;
  }

  void DockerSplit::AddContainer(int index, DockerContainer* container)
  {
    children.insert(children.begin() + index, container);

    int vecIndex = vertical ? 1 : 0;

    Vec2 containerSize = size;
    containerSize[vecIndex] = size[vecIndex] / (children.size()-1);
    container->SetParent(this);
    container->SetSize(containerSize);
    SetSize(size);
    SetPosition(position);
  }

  void DockerSplit::RemoveDocker(int index)
  {
    ASSERT(index >= 0 && index < children.size(), "Index out of bound");
    DockerInterface* interface = *(children.begin() + index);
    children.erase(children.begin() + index);
    delete interface;
    if(children.size() == 0)
    {
      if(parent != nullptr)
      {
        DockerSplit* split = static_cast<DockerSplit*>(parent);
        static_cast<DockerSplit*>(split)->RemoveDocker(split->GetDockerIndex(this));
      }
    }
    else
    {
      // Resize other components
      SetSize(size);
      SetPosition(position);
    }
  }

  int DockerSplit::GetDockerIndex(DockerInterface* interface)
  {
    for(int i = 0;i<children.size(); ++i)
    {
      if(children[i] == interface)
        return i;
    }
    ASSERT(false, "Docker is not inside DockerSplit");
    return children.size();
  }

  int DockerSplit::GetDockerCount() const
  {
    return children.size();
  }

  Component* DockerSplit::GetComponentByNameNoCast(const std::string& name)
  {
    for(auto&& child : children)
    {
      Component* comp = child->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  bool DockerSplit::IsVertical() const
  {
    return vertical;
  }

  void DockerSplit::SetGUIScene(GUIScene* scene)
  {
    for(auto&& child : children)
    {
      child->SetGUIScene(scene);
    }
  }

  void DockerSplit::SetPosition(const Vec2& _position)
  {
    position = _position;
    Vec2 offset = position;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->SetPosition({floor(offset.x), floor(offset.y)});
      offset[vecIndex] += child->GetSize()[vecIndex];
    }
  }

  void DockerSplit::SetSize(const Vec2& _size)
  {
    size = _size;
    Vec2 sumSize{0,0};
    int vecIndex = vertical ? 1 : 0;

    for(auto&& child : children)
    {
      sumSize[vecIndex] += child->GetSize()[vecIndex];
    }
    for(auto&& child : children)
    {
      Vec2 childSize = _size;
      childSize[vecIndex] = floor(_size[vecIndex] * (child->GetSize()[vecIndex] / sumSize[vecIndex]));
      child->SetSize(childSize);
    }
  }
}
