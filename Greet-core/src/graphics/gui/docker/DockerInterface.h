#pragma once

#include <event/Event.h>
#include <graphics/gui/GUIScene.h>
#include <graphics/renderers/GUIRenderer.h>
#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  class Docker;
  class DockerTab;

  class DockerInterface
  {
    protected:
      Docker* docker;
      DockerInterface* parent;
      Vec2f position;
      Vec2f size;
      float weight = 1.0f;

    public:
      DockerInterface(Docker* docker, DockerInterface* parent);
      virtual ~DockerInterface(){}


      const Vec2f& GetPosition() const { return position; }
      const Vec2f& GetSize() const { return size; }
      bool IsMouseInside(const Vec2f& mousePos)
      {
        return mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < size.x && mousePos.y < size.y;
      }

      float GetWeight() const { return weight; }
      void SetWeight(float _weight) { weight = _weight; }
      void SetParent(DockerInterface* interface) { parent = interface; }

    public:
      virtual void Render(GUIRenderer* renderer) const = 0;
      virtual void Update(float timeElapsed) = 0;
      virtual void OnEvent(Event& event, const Vec2f& translation) = 0;

      virtual void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2f& componentPos) = 0;

      virtual Component* GetComponentByNameNoCast(const std::string& name) = 0;
      virtual Vec2f GetMinSize() const = 0;

      virtual void LoadFrameStyle(const MetaFile& metaFile) = 0;

      virtual void SetPosition(const Vec2f& position) = 0;
      virtual void SetSize(const Vec2f& avSize, bool abRemeasure) = 0;
      virtual void SetGUIScene(GUIScene* scene) = 0;

      virtual void DebugPrint(int indent) = 0;

      virtual DockerTab* GetTab(const std::string& tabName) const = 0;
  };
}

