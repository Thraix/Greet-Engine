#pragma once

#include <ostream>
#include <string>

namespace Greet {

  struct FileUtils {

    struct TimeModified
    {
      timespec time;
      TimeModified(const timespec& time)
        : time{time}{}
      TimeModified()
      {}

      bool operator<(const TimeModified& rhs)
      {
        if (time.tv_sec == rhs.time.tv_sec)
          return time.tv_nsec < rhs.time.tv_nsec;
        else
          return time.tv_sec < rhs.time.tv_sec;
      }

      friend std::ostream& operator<<(std::ostream& stream, const TimeModified& time)
      {
        return stream << time.time.tv_sec << "." << time.time.tv_nsec;
      }
    };

    static void PrintWorkingDirectory();
    static TimeModified GetTimeModified(const std::string& filename);
    static std::string ReadFile(const std::string& filepath);
    static void WriteFile(const std::string& filepath, const std::string& write);
    static bool FileExist(const std::string& filepath);
    static std::string ReplaceExtension(const std::string& file, const std::string& ext);
  };
}
