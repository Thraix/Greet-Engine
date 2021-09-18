#include "UUID.h"

#include <logging/Log.h>

namespace Greet {

  UUID UUID::s_instance;

  UUID::UUID()
    : m_randomEngine(time(NULL)), m_distribution()
  {
  }

  uint32_t UUID::GetUUID()
  {
    return GenNewUUID();
  }

  // generates a random 32 bit number
  uint32_t UUID::GetRandomNumber()
  {
    return m_distribution(m_randomEngine);
  }

  uint32_t UUID::GenNewUUID()
  {
    uint32_t i = 1000;
    uint32_t number;
    while (i--)
    {
      number = GetRandomNumber();
      if (number != 0 && m_usedUUID.count(number) == 0)
      {
        m_usedUUID.insert(number);
        return number;
      }
    }
    Log::Warning("Could not generate random UUID. Try again.");
    return 0;
  }
}
