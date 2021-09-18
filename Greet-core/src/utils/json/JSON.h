#pragma once

#include <logging/Log.h>

// TODO: Move implementation to cpp file

namespace Greet {

  class JSON
  {
    private:
      static std::string m_indent;

    public:
      static const std::string& get_indent()
      {
        return m_indent;
      }

      static void push_indent()
      {
        m_indent.push_back('\t');
      }

      static void pop_indent()
      {
        ASSERT(m_indent.size() != 0, "Trying to pop JSON indent that is empty");
        m_indent.pop_back();
      }
  };
}
