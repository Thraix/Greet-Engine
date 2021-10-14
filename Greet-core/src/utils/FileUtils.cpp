#include "FileUtils.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define stat _stat
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <logging/Log.h>

namespace Greet {

    void FileUtils::PrintWorkingDirectory()
    {
      char cCurrentPath[FILENAME_MAX];

      if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
        Log::Error("Cannot print working directory");
        return;
      }
      Log::Info(cCurrentPath);
    }

    FileUtils::TimeModified FileUtils::GetTimeModified(const std::string& filename)
    {
      struct stat attrib;
      stat(filename.c_str(), &attrib);
      return TimeModified{attrib.st_mtim};
    }

    std::string FileUtils::ReadFile(const std::string& filepath)
    {
      FILE *file = fopen(filepath.c_str(), "rt");
      if (!file)
      {
        Log::Error("File could not be read: ", filepath);
        return "";
      }
      fseek(file, 0, SEEK_END);
      unsigned long length = ftell(file);
      char* data = new char[length + 1];
      memset(data, 0, length + 1);
      fseek(file, 0, SEEK_SET);
      fread(data, 1, length, file);
      fclose(file);
      std::string result(data);
      delete[] data;
      return result;
    }

    void FileUtils::WriteFile(const std::string& filepath, const std::string& write)
    {
      FILE *file = fopen(filepath.c_str(),"wt");
      fseek(file, 0, SEEK_END);
      fputs(write.c_str(),file);
      fclose(file);
    }

    bool FileUtils::FileExist(const std::string& filepath)
    {
      struct stat attrib;
      if(stat(filepath.c_str(), &attrib) != -1)
        return S_ISREG(attrib.st_mode);
      return false;
    }

    std::string FileUtils::ReplaceExtension(const std::string& file, const std::string& ext)
    {
      size_t pos = file.find_last_of('.');
      if(pos == std::string::npos)
        return file + "." + ext;
      return file.substr(0, pos + 1) + ext;
    }
}
