#include "Editor.h"

#include "entity/EntityManager.h"

#include <graphics/GlobalSceneManager.h>
#include <graphics/fonts/FontManager.h>
#include <graphics/gui/ComponentFactory.h>
#include <graphics/gui/GUIScene.h>
#include <input/Input.h>

namespace Greet
{
  Editor::Editor()
    : App{"Editor", 1440, 1440 * 9 / 16}
  {
    SetFrameCap(60);
  }

  Editor::~Editor()
  {
  }

  void Editor::Init()
  {
    FontManager::Add("noto", FontContainer("res/fonts/noto.ttf"));
    gui = NewRef<GUIScene>();
    Frame* frame = FrameFactory::GetFrame("res/guis/Editor.xml");
    gui->AddFrameQueued(frame);
    GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");

    entityManager = NewRef<EntityManager>(frame);
  }

  void Editor::Destruct()
  {
    GlobalSceneManager::GetSceneManager().Remove2DScene("gui");
    entityManager->Serialize("res/scenes/game.meta");
    entityManager.reset();
    gui.reset();
  }

  void Editor::Tick()
  {
  }

  void Editor::Render()
  {
  }

  void Editor::Update(float timeElapsed)
  {
  }

  void Editor::OnEvent(Event& event)
  {
    if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
    {
      KeyPressEvent& e = static_cast<KeyPressEvent &>(event);
      if(Input::IsKeyDown(GREET_KEY_LEFT_CONTROL) && e.GetButton() == GREET_KEY_S)
      {
        Log::Info("Saving...");
        entityManager->Serialize("res/scenes/game.meta");
      }
    }
  }
}
