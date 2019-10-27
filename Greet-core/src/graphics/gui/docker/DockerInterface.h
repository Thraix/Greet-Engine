#pragma once

#include <event/Event.h>
#include <graphics/gui/GUIScene.h>
#include <graphics/renderers/GUIRenderer.h>
#include <math/Vec2.h>

#define TAB_WIDTH 100
#define TAB_HEIGHT 20
#define TAB_PADDING 5
namespace Greet
{
  class Docker;
  class DockerTab;

  class DockerInterface
  {
    protected:
      Docker* docker;
      DockerInterface* parent;
      Vec2 position;
      Vec2 size;
    public:
      DockerInterface(Docker* docker, DockerInterface* parent);
      virtual ~DockerInterface(){}


      const Vec2& GetPosition() const { return position; }
      const Vec2& GetSize() const { return size; }
      bool IsMouseInside(const Vec2& mousePos)
      {
        return mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < size.x && mousePos.y < size.y;
      }

      void SetParent(DockerInterface* interface) { parent = interface; }

    public:
      virtual void Render(GUIRenderer* renderer) const = 0;
      virtual void Update(float timeElapsed) = 0;
      virtual void OnEvent(Event& event, const Vec2& translation) = 0;

      // Returns true if the tab should be removed from its current container
      virtual bool HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) = 0;

      virtual Component* GetComponentByNameNoCast(const std::string& name) = 0;

      virtual void SetPosition(const Vec2& position) = 0;
      virtual void SetSize(const Vec2& position) = 0;
      virtual void SetGUIScene(GUIScene* scene) = 0;
  };
}

