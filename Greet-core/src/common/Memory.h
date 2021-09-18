#pragma once

#include <memory>

#define DELETE_COPY_AND_MOVE(CLASS_NAME) \
  CLASS_NAME(const CLASS_NAME&) = delete; \
  CLASS_NAME(CLASS_NAME&&) = delete; \
  CLASS_NAME& operator=(const CLASS_NAME&) = delete; \
  CLASS_NAME& operator=(CLASS_NAME&&) = delete;

#define DELETE_CONSTRUCTOR_DESTRUCTOR(CLASS_NAME) \
  CLASS_NAME() = delete; \
  ~CLASS_NAME() = delete; \

namespace Greet
{
  template <typename T>
  using Ref = std::shared_ptr<T>;

  template <typename T, typename... Args>
  Ref<T> NewRef(const Args&... args)
  {
    return std::make_shared<T>(args...);
  }

  template<class T> struct RefLess
  {
    bool operator()(const Ref<T>& lhs, const Ref<T> rhs) const
    {
      return lhs.get() < rhs.get();
    }
  };

  template <typename T>
  using Ptr = std::unique_ptr<T>;

  template <typename T, typename... Args>
  Ptr<T> NewPtr(const Args&... args)
  {
    return std::make_unique<T>(args...);
  }

  template<class T> struct PtrLess
  {
    bool operator()(const Ptr<T>& lhs, const Ptr<T> rhs) const
    {
      return lhs.get() < rhs.get();
    }
  };
}
