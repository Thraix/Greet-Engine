#pragma once

#include <string>

namespace Greet {

  class JSON
  {
    private:
      static std::string m_indent;

    public:
      static const std::string& GetIndent();
      static void PushIndent();
      static void PopIndent();
  };
}
