#pragma once

#include <random>
#include <set>
#include <stdint.h>

namespace Greet {


  class UUID
  {
    public:
      friend class Window;
    private:
      std::default_random_engine m_randomEngine;
      std::uniform_int_distribution<uint32_t> m_distribution;
      std::set<uint32_t> m_usedUUID;

    public:
      uint32_t GetUUID();
    public:
      static UUID& GetInstance() { return s_instance; }
    private:
      UUID();
      ~UUID() { }
      uint32_t GetRandomNumber();
      uint32_t GenNewUUID();
      static UUID s_instance;
  };
}
