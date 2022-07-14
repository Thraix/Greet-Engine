#pragma once

#include <random>
#include <set>
#include <stdint.h>

namespace Greet {


  class UUID final
  {
    private:
      static std::default_random_engine randomEngine;
      static std::uniform_int_distribution<uint64_t> distribution;

    public:
      UUID();
      UUID(uint64_t msb, uint64_t lsb);
      // Format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
      UUID(const std::string& s);
      uint64_t msb;
      uint64_t lsb;

      uint64_t GetMsb();
      uint64_t GetLsb();
      std::string GetString() const;

      friend bool operator<(const UUID& lhs, const UUID& rhs);
      friend bool operator==(const UUID& lhs, const UUID& rhs);
      friend bool operator!=(const UUID& lhs, const UUID& rhs);
      friend std::ostream& operator<<(std::ostream& stream, const UUID& uuid);
  };
}
