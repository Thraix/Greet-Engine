#pragma once

#include <drivers/Driver.h>

#include <vector>

namespace Greet {
  class App;
}

namespace Greet {

  class DriverDispatcher
  {
    public:
      friend class App;
      static void AddDriver(Driver* driver);

    private:
      static std::vector<Driver*> m_drivers;
      static void Update(float timeElapsed);


  };

}
