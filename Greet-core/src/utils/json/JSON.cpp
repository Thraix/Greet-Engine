#include "JSON.h"

#include <logging/Log.h>

namespace Greet {
  std::string JSON::m_indent;

  const std::string& JSON::GetIndent()
  {
    return m_indent;
  }

  void JSON::PushIndent()
  {
    m_indent.push_back('\t');
  }

  void JSON::PopIndent()
  {
    ASSERT(m_indent.size() != 0, "Trying to pop JSON indent that is empty");
    m_indent.pop_back();
  }
}
