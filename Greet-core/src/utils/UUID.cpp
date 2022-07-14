#include "UUID.h"

#include <utils/LogUtils.h>
#include <iomanip>

namespace Greet {

  std::default_random_engine UUID::randomEngine{std::random_device{}()};
  std::uniform_int_distribution<uint64_t> UUID::distribution;

  UUID::UUID()
    : msb{distribution(randomEngine)}, lsb{distribution(randomEngine)}
  {}

  UUID::UUID(uint64_t msb, uint64_t lsb)
    : msb{msb}, lsb{lsb}
  {}

  UUID::UUID(const std::string& s)
  {
    std::regex regex{"[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}"};
    if(!std::regex_match(s, regex))
    {
      Log::Warning("Invalid UUID format \"%s\"", s);
      Log::Warning("Valid format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx");
      Log::Warning("where x is 0-9 or a-f");
      msb = 0;
      lsb = 0;
      return;
    }

    std::stringstream ss{s};
    char c;

    // Standard UUID format names
    uint64_t time_low;
    uint64_t time_mid;
    uint64_t time_hi_and_version;
    uint64_t clock_seq_hi_and_res_clock_seq_low;
    uint64_t node;

    ss >> std::hex >> time_low >> c >> time_mid >> c >> time_hi_and_version >> c >> clock_seq_hi_and_res_clock_seq_low >> c >> node;
    msb = (time_low << 32) | (time_mid << 16) | time_hi_and_version;
    lsb = (clock_seq_hi_and_res_clock_seq_low << 48) | node;
  }

  bool operator<(const UUID& lhs, const UUID& rhs)
  {
    if(lhs.msb == rhs.msb)
      return lhs.lsb < rhs.lsb;
    return lhs.msb < rhs.msb;
  }

  bool operator==(const UUID& lhs, const UUID& rhs)
  {
    return lhs.msb == rhs.msb && lhs.lsb == rhs.lsb;
  }

  bool operator!=(const UUID& lhs, const UUID& rhs)
  {
    return !(lhs == rhs);
  }

  uint64_t UUID::GetMsb()
  {
    return msb;
  }

  uint64_t UUID::GetLsb()
  {
    return lsb;
  }

  std::string UUID::GetString() const
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }


  std::ostream& operator<<(std::ostream& stream, const UUID& uuid)
  {
    stream << std::hex << std::setfill('0');
    stream << std::setw(8) << ((uuid.msb >> 32) & 0xffffffff) << '-';
    stream << std::setw(4) <<  ((uuid.msb >> 16) & 0xffff) << '-';
    stream << std::setw(4) << (uuid.msb & 0xffff) << '-';
    stream << std::setw(4) << ((uuid.lsb >> 48) & 0xffff) << '-';
    stream << std::setw(12) << (uuid.lsb & 0xffffffffffff);

    stream << std::dec;
    return stream;
  }
}
