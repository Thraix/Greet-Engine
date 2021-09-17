#include "App.h"
#include <drivers/DriverDispatcher.h>
#include <chrono>
#include <thread>
#include <logging/Log.h>
#include <graphics/GlobalSceneManager.h>
#include <utility>
#include <functional>
#include <event/EventDispatcher.h>
#include <input/Input.h>
#include <graphics/RenderCommand.h>

namespace Greet {

  App::App(const std::string& title, uint width, uint height)
    : m_fps(0), m_ups(0)
  {
    using namespace std::placeholders;
    EventDispatcher::AddGlobalEventReceiver("Application", std::bind(&App::OnEvent, std::ref(*this), _1));
    Window::CreateWindow(title,width,height);
    //std::ofstream file(".logging");
    //LogStream* fileStream = new LogStream(file, LogLevel::error, "fileStream");
    //Log::addLogStream(fileStream);
  }

  App::~App()
  {
    Window::DestroyWindow();
  }

  void App::Start()
  {
    Init();
    Run();
    Destruct();
  }

  void App::Run()
  {
    m_timer = new Timer();
    double timer = 0.0f;
    double updateTimer = 0.0f;
    double updateTick = frameCap;
    double renderTimer = 0.0f;

    uint frames = 0;
    uint updates = 0;

    while (!Window::Closed())
    {
      double elapsed = m_timer->Elapsed();
      if (elapsed - updateTimer >= updateTick)
      {
        Timer timer;
        Window::Update();
        DriverDispatcher::Update(updateTick);
        GlobalSceneManager::GetSceneManager().Update(elapsed - updateTimer);
        Update(elapsed - updateTimer);
        updates++;
        updateTimer = elapsed;
        Input::Update();
      }

      elapsed = m_timer->Elapsed();
      if (elapsed - renderTimer >= frameCap)
      {
        RenderCommand::Clear();
        GlobalSceneManager::GetSceneManager().Render();
        Render();
        Window::Render();
        frames++;
        renderTimer = elapsed;
      }

      elapsed = m_timer->Elapsed();
      if (elapsed - timer >= 1.0)
      {
        m_fps = frames;
        m_ups = updates;
        Window::Tick();
        Tick();
        frames = 0;
        updates = 0;
        timer += 1.0;
      }
      // This is generally bad and should probably not be here, but it makes the
      // computer work less and therefore draw less energy/lower temps
      elapsed = m_timer->Elapsed();
      double timeToNext = fmin(frameCap - (elapsed - renderTimer),updateTick - (elapsed-updateTimer))*1000*0.5;
      if (timeToNext >= 1)
      {
        if(timeToNext > 100)
          Log::Info("Sleeping for %s milliseconds", timeToNext);
        std::this_thread::sleep_for(std::chrono::milliseconds((long long)timeToNext));
      }
    }
    delete m_timer;
  }
}
