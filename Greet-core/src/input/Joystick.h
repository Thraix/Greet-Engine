#pragma once

#include <vector>
#include <internal/GreetTypes.h>
#include <math/Maths.h>
#include <internal/GreetGL.h>

#ifndef MAX_JOYSTICKS 
#define MAX_JOYSTICKS 4
#endif


namespace Greet {

  class Joystick
  {
    public:
      friend class Window;

    private:
      bool m_connected = false;
      bool m_wasConnected = false;

      // Joystick number
      const int m_jsNum;

      // Axis Data
      int axisCount; 
      const float* axis;
      float* previousAxis;

      // Button data
      int buttonCount; 
      unsigned char* previousButtons;
      const unsigned char* buttons;

    private:
      Joystick(uint jsNum, float m_calibrateLeft, float m_calibrateRight);
      void SetState(bool connected);
      void Update();
      bool CheckConnect();
    public:
      virtual ~Joystick();
      bool IsConnected() const { return m_connected; }

  };
}
