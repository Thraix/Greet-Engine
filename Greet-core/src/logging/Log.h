#pragma once

#include <logging/Logger.h>
#include <logging/LogLevel.h>
#include <string.h>

namespace Greet {
  /*
     Singleton class for logging
     */
  class Log
  {
    private:
      // Logger for the logging system
      Logger m_logger;

      // Singleton Log
      static Log m_log;
    public:

    private:
      Log();
      virtual ~Log();
    public:

      /*
       * Add a log stream to the logging system.
       */
      static void AddLogStream(LogStream* stream);

      /*
       * Remove a log stream from the logging system.
       */
      static LogStream* RemoveLogStream(const std::string& name);

      /*
       * Remove a log stream from the logging system.
       */
      static LogStream* RemoveLogStream(LogStream* stream);

      // Default logging capabilities

      /*
       * Logging at information log level.
       */
      template <typename... Args>
      static void Info(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Information(), "[Greet][INF] ");
        m_log.m_logger.Log(LogLevel::Information(), args..., "\n");
      }

      /*
       * Logging at warning log level.
       */
      template <typename... Args>
      static void Warning(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Warning(), "\e[0;33m[Greet][WRN] ");
        m_log.m_logger.Log(LogLevel::Warning(), args...);
        m_log.m_logger.Log(LogLevel::Warning(), "\n\e[m");
      }

      /*
       * Logging at error log level.
       */
      template <typename... Args>
      static void Error(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Warning(), "\e[0;31m[Greet][ERR] ");
        m_log.m_logger.Log(LogLevel::Warning(), args...);
        m_log.m_logger.Log(LogLevel::Warning(), "\n\e[m");
      }

      /*
       * Logging at assert log level.
       */
      template <typename... Args>
      static void LogAssert(const char* filename, const char* funcname, int linenr, const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Assert(), "\e[0;31m[Greet][ASSERT][%s::%s:%s] ", filename, funcname, linenr);
        m_log.m_logger.Log(LogLevel::Assert(), args...);
        m_log.m_logger.Log(LogLevel::Assert(), "\n\e[m");
      }
  };
}

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#if _DEBUG
#define ASSERT(x,...) \
if(!(x)) \
{ \
  Greet::Log::LogAssert(__FILENAME__, __func__, __LINE__, __VA_ARGS__);\
  abort();\
}
#else
#define ASSERT(x,...)
#endif
