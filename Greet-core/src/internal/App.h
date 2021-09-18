#pragma once

#include <common/Memory.h>
#include <utils/Timer.h>
#include <event/Event.h>

namespace Greet {

  class App
  {
    DELETE_COPY_AND_MOVE(App);

    protected:
      Timer* m_timer;
      uint32_t m_fps;
      uint32_t m_ups;
      double frameCap = 0;
    protected:
      App(const std::string& title, uint32_t width, uint32_t height);

      virtual ~App();

      virtual void Init() = 0;
      virtual void Destruct() = 0;
      virtual void Tick() = 0;
      virtual void Update(float elapsedTime) = 0;
      virtual void Render() = 0;
      virtual void OnEvent(Event& event) {}

      void SetFrameCap(uint32_t frameCap)
      {
        if (frameCap == 0)
          this->frameCap = 0;
        else
          this->frameCap = 1.0 / (double)frameCap;
      }

      uint32_t GetFrameCap()
      {
        if (frameCap == 0)
          return 0;
        return (uint32_t)(1.0 / frameCap);
      }

      uint32_t GetFPS() const { return m_fps; }
      uint32_t GetUPS() const { return m_ups; }

    public:
      void Start();

    private:
      void Run();
  };
}
