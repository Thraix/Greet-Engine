#include "DockerSplit.h"

#include <graphics/gui/Docker.h>
#include <graphics/gui/docker/DockerContainer.h>

#include <chrono>
#include <sstream>
#include <thread>

namespace Greet
{
  DockerSplit::DockerSplit(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}
  {
    vertical = GUIUtils::GetBooleanFromXML(object, "vertical", false);
    weight = GUIUtils::GetFloatFromXML(object, "weight", 1.0);
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
    NormalizeWeights();
  }

  DockerSplit::DockerSplit(DockerSplit* split, Docker* docker, DockerSplit* parent, bool vertical)
    : DockerInterface{docker, parent}, children{split}, vertical{vertical}
  {
    split->SetParent(this);
    NormalizeWeights();
  }

  DockerSplit::DockerSplit(const std::vector<DockerInterface*>& children, Docker* docker, DockerSplit* parent, bool vertical)
    : DockerInterface{docker, parent}, children{children}, vertical{vertical}
  {
    for(auto child : children)
    {
      child->SetParent(this);
    }
    NormalizeWeights();
  }

  DockerSplit::~DockerSplit()
  {
    for(auto&& child : children)
      delete child;
  }

  void DockerSplit::Render(GUIRenderer* renderer) const
  {
    for(int i = 0; i < children.size()-1;++i)
    {
      int vecIndex = vertical ? 1 : 0;
      Vec2f edgePos = docker->GetPosition() + position;
      Vec2f edgeSize = size;
      edgePos[vecIndex] = children[i]->GetPosition()[vecIndex] + children[i]->GetSize()[vecIndex];
      edgeSize[vecIndex] = docker->edgeWidth;

      renderer->DrawRect(edgePos, edgeSize, docker->edgeBorderColor, false);
      edgePos[vecIndex] += docker->edgeBorderSize;

      edgeSize[vecIndex] -= 2*docker->edgeBorderSize;
      renderer->DrawRect(edgePos, edgeSize, docker->edgeColor, false);
    }
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

  void DockerSplit::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      Vec2f mousePos = e.GetPosition() - componentPos;
      if((vertical && mousePos.x >= 0 && mousePos.x < size.w) || (!vertical && mousePos.y >= 0 && mousePos.y < size.h))
      {
        int vecIndex = vertical ? 1 : 0;
        for(int i = 0;i<children.size()-1;i++)
        {
          mousePos[vecIndex] -= children[i]->GetSize()[vecIndex] + docker->edgeWidth;
          if(mousePos[vecIndex] >= -docker->edgeWidth && mousePos[vecIndex] < 0)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
            grabPos = e.GetPosition()[vecIndex] - componentPos[vecIndex];
            grabSize = children[i]->GetSize()[vecIndex];
            event.AddFlag(EVENT_FOCUSED | EVENT_HANDLED);
            docker->RequestFocus();
            return;
          }
        }
      }
      Vec2f pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        if(child->IsMouseInside(e.GetPosition() - pos))
          child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(grabbingEdge)
      {
        Vec2f mousePos = e.GetPosition() - componentPos;

        DockerInterface* dockerThis = children[grabbedEdgeIndex];
        DockerInterface* dockerNext = children[grabbedEdgeIndex+1];
        float oldWeightThis = children[grabbedEdgeIndex]->GetWeight();
        float oldWeightNext = children[grabbedEdgeIndex+1]->GetWeight();

        Vec2f sizeThis = dockerThis->GetSize();
        Vec2f sizeNext = dockerNext->GetSize();

        int vecIndex = vertical ? 1 : 0;

        float offset = mousePos[vecIndex] - grabPos;

        float totalWeight = (oldWeightThis + oldWeightNext);
        float totalSize = sizeThis[vecIndex] + sizeNext[vecIndex];

        sizeThis[vecIndex] = grabSize + offset;
        float minSizeThis = dockerThis->GetMinSize()[vecIndex];
        float minSizeNext = dockerNext->GetMinSize()[vecIndex];
        Math::Clamp(&sizeThis[vecIndex], minSizeThis, totalSize - minSizeNext);


        float weightThis = totalWeight * sizeThis[vecIndex] / totalSize;

        dockerThis->SetWeight(weightThis);
        dockerNext->SetWeight(totalWeight - weightThis);

        SetSize(size, false);
        return;
      }
      Vec2f pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      if(grabbingEdge)
      {
        grabbingEdge = false;
        event.AddFlag(EVENT_UNFOCUSED);
        return;
      }
      Vec2f pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      for(auto&& child : children)
      {
        child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex];
      }
      return;
    }

    Vec2f pos = componentPos;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->OnEvent(event, pos);
      pos[vecIndex] += child->GetSize()[vecIndex];
    }
  }

  void DockerSplit::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2f& componentPos)
  {
    Vec2f mousePos = event.GetPosition() - componentPos;
    for(auto&& child : children)
    {
      if(Utils::IsInside(mousePos, child->GetPosition(), child->GetSize()))
      {
        child->HandleDroppedTab(tab, event, componentPos);
        return;
      }
    }
  }

  void DockerSplit::AddDocker(int index, DockerInterface* interface)
  {
    ASSERT(index >= 0 && index <= children.size(), "Index out of bound");
    children.insert(children.begin() + index, interface);
    interface->SetParent(this);
    docker->MarkDirty();
  }

  DockerInterface* DockerSplit::RemoveDocker(int index)
  {
    ASSERT(index >= 0 && index < children.size(), "Index out of bound");
    DockerInterface* interface = *(children.begin() + index);
    children.erase(children.begin() + index);
    docker->MarkDirty();
    return interface;
  }

  void DockerSplit::ReplaceDocker(int index, DockerInterface* interface)
  {
    ASSERT(index >= 0 && index < children.size(), "Index out of bound");
    children[index] = interface;
    docker->MarkDirty();
  }

  void DockerSplit::MergeSimilarSplits()
  {
    // It is probably possible to merge these two for loops into a single one
    // But the readability might get compicated.


    // Remove unnecessary Splits and empty Containers
    for(int i = 0;i<children.size();++i)
    {
      DockerInterface* dock = children[i];
      if(DockerSplit* split = dynamic_cast<DockerSplit*>(dock))
      {
        split->MergeSimilarSplits();
        if(vertical == split->vertical || split->children.size() == 1)
        {
          for(int j = 0;j<split->children.size();++j)
          {
            AddDocker(i+j+1, split->children[j]);
          }
          split->children.clear();
          delete RemoveDocker(i);
          --i;
        }
      }
      else if(DockerContainer* container = dynamic_cast<DockerContainer*>(dock))
      {
        if(container->GetTabCount() == 0)
        {
          delete RemoveDocker(i);
          --i;
        }
      }
    }

    // Fix positions and sizes
    for(int i = 0;i<children.size();++i)
    {
      int vecIndex = vertical ? 1 : 0;
      Vec2f p = children[i]->GetPosition();
      Vec2f s = children[i]->GetSize();
      p[1-vecIndex] = GetPosition()[1-vecIndex];
      s[1-vecIndex] = GetSize()[1-vecIndex];

      if(i == 0)
      {
        // Move back if first element was removed
        s[vecIndex] += p[vecIndex] - GetPosition()[vecIndex];
        p[vecIndex] = GetPosition()[vecIndex];
      }
      if(i+1 < children.size())
      {
        // Increase the size to match the next dockers position
        Vec2f p2 = children[i+1]->GetPosition();
        s[vecIndex] = p2[vecIndex] - p[vecIndex] - docker->edgeWidth;
      }
      else
      {
        // Increase the size to match the end of the this dockers pos+size
        s[vecIndex] = GetPosition()[vecIndex] + GetSize()[vecIndex] - p[vecIndex];
      }
      children[i]->SetPosition(p);
      children[i]->SetSize(s, true);
    }

    UpdateWeights();
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

  Vec2f DockerSplit::GetMinSize() const
  {
    Vec2f minSize = {0, 0};
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      Vec2f minSizeChild = child->GetMinSize();
      minSize[vecIndex] += minSizeChild[vecIndex];
      minSize[1-vecIndex] = fmax(minSize[1-vecIndex], minSizeChild[1-vecIndex]);
    }
    minSize[vecIndex] += (children.size() - 1) * docker->edgeWidth;
    return minSize;
  }

  DockerTab* DockerSplit::GetTab(const std::string& tabName) const
  {
    for(auto&& child : children)
    {
      DockerTab* tab = child->GetTab(tabName);
      if(tab != nullptr)
        return tab;
    }
    return nullptr;
  }

  void DockerSplit::LoadFrameStyle(const MetaFile& metaFile)
  {
    for(auto&& child : children)
    {
      child->LoadFrameStyle(metaFile);
    }
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

  void DockerSplit::SetPosition(const Vec2f& _position)
  {
    position = _position;
    Vec2f offset = position;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->SetPosition({floor(offset.x), floor(offset.y)});
      offset[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
    }
  }

  void DockerSplit::SetSize(const Vec2f& avSize, bool abRemeasure)
  {
    size = avSize;
    int vecIndex = vertical ? 1 : 0;
    float totalWeight = children.size();
    int edgeWidths = docker->edgeWidth * (children.size() - 1);
    Vec2f pos = position;
    float usedSize = 0.0f;

    for(int i = 0; i < children.size() - 1; i++)
    {
      Vec2f childSize = avSize;
      childSize[vecIndex] = round((avSize[vecIndex] - edgeWidths) * children[i]->GetWeight() / totalWeight);
      children[i]->SetSize(childSize, abRemeasure);
      children[i]->SetPosition(pos);
      usedSize += childSize[vecIndex] + docker->edgeWidth;
      pos[vecIndex] += childSize[vecIndex] + docker->edgeWidth;
    }

    // Add the unused space to the last component
    DockerInterface* last = children[children.size() - 1];
    Vec2f childSize = avSize;
    childSize[vecIndex] = size[vecIndex] - usedSize;
    last->SetSize(childSize, abRemeasure);
    last->SetPosition(pos);
  }


  void DockerSplit::UpdateWeights()
  {
    int vecIndex = vertical ? 1 : 0;

    float size = GetSize()[vecIndex] - docker->edgeWidth * (children.size() - 1);

    float totalWeight = children.size();
    for(auto child : children)
    {
      child->SetWeight(child->GetSize()[vecIndex] / size * totalWeight);
    }
  }

  void DockerSplit::NormalizeWeights()
  {
    float totalWeight = 0.0f;
    for(auto&& child : children)
    {
      totalWeight += child->GetWeight();
    }
    float weightScale = children.size() / totalWeight;
    for(auto&& child : children)
    {
      child->SetWeight(child->GetWeight() * weightScale);
    }
  }

  void DockerSplit::DebugPrint(int indent)
  {
    std::stringstream ss;
    for(int i = 0;i<indent;i++)
    {
      ss << "-";
    }
    if(vertical)
      ss << "[vertical]";
    else
      ss << "[horizontal]";
    ss << " w=" << weight;
    Log::Info(ss.str());
    for(auto it = children.begin(); it != children.end();++it)
    {
      (*it)->DebugPrint(indent+1);
    }
  }
}
