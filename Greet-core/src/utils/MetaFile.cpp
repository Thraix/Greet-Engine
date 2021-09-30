#include "MetaFile.h"

#include <fstream>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
  MetaFileClass::MetaFileClass(const std::string& className)
    : className{className}
  {}

  std::string MetaFileClass::GetValue(const std::string& key, const std::string& val) const
  {
    auto it = values.find(key);
    if(it != values.end())
      return it->second;
    return val;
  }

  bool MetaFileClass::HasValue(const std::string& key) const
  {
    return values.find(key) != values.end();
  }

  const std::string& MetaFileClass::GetValue(const std::string& key) const
  {
    auto it = values.find(key);
    ASSERT(it != values.end(), "Values does not exist in meta file");
    return it->second;
  }

  const std::unordered_map<std::string, std::string>& MetaFileClass::GetValues() const
  {
    return values;
  }

  std::ostream& operator<<(std::ostream& stream, const MetaFileClass& file)
  {
    stream << "[" << file.className << "]" << std::endl;
    for(auto value : file.GetValues())
    {
      stream << value.first << "=" << value.second << std::endl;
    }
    return stream;
  }

  MetaFile::MetaFile() {}
  MetaFile::MetaFile(const std::string& filepath)
    : filepath{filepath}
  {
    if(StringUtils::EndsWith(filepath, ".meta"))
    {
      std::ifstream stream(filepath);

      if(!stream.is_open())
      {
        Log::Error("Could not find meta file: ", filepath);
        return;
      }
      LoadMetaFile(stream);
    }
  }

  MetaFile::MetaFile(std::istringstream&& stream)
  {
    LoadMetaFile(stream);
  }

  bool MetaFile::HasMetaClass(const std::string& className)
  {
    return classes.find(className) != classes.end();
  }

  std::vector<MetaFileClass>& MetaFile::GetMetaClass(const std::string& className)
  {
    static std::vector<MetaFileClass> empty;
    auto it = classes.find(className);
    if(it != classes.end())
      return it->second;
    return empty;
  }

  const std::vector<MetaFileClass>& MetaFile::GetMetaClass(const std::string& className) const
  {
    static std::vector<MetaFileClass> empty;
    auto it = classes.find(className);
    if(it != classes.end())
      return it->second;
    return empty;
  }

  std::ostream& operator<<(std::ostream& stream, const MetaFile& file)
  {
    for(auto metaClasses : file.classes)
    {
      for(auto metaClass : metaClasses.second)
      {
        stream << metaClass;
      }
    }
    return stream;
  }

  void MetaFile::LoadMetaFile(std::istream& stream)
  {
    std::string currentClass = "";
    auto metaClassIt = classes.end();
    std::string line;
    while(std::getline(stream, line))
    {
      if(line.empty())
        continue;

      if(line[StringUtils::GetTrimStartPos(line)] == '[' && line[StringUtils::GetTrimEndPos(line)] == ']' )
      {
        currentClass = StringUtils::Trim(line);
        currentClass = currentClass.substr(1, currentClass.size() - 2);
        metaClassIt = classes.find(currentClass);
        if(metaClassIt == classes.end())
          metaClassIt = classes.emplace(currentClass, std::vector<MetaFileClass>{{currentClass}}).first;
        else
          metaClassIt->second.push_back({currentClass});
        continue;
      }

      size_t pos = line.find("=");
      if(pos == std::string::npos)
      {
        Log::Warning("Meta file line does not contain \'=\'");
        continue;
      }

      std::string_view key = StringUtils::Trim(std::string_view(line.c_str(), pos));
      std::string_view value = StringUtils::Trim(std::string_view(line.c_str() + pos + 1));
      if(key.length() == 0)
      {
        Log::Warning("Meta file key is empty");
        continue;
      }
      if(metaClassIt != classes.end())
      {
        auto res = metaClassIt->second[metaClassIt->second.size()-1].values.emplace(key, value);
        if(!res.second)
        {
          Log::Warning("Meta file key is defined twice: ", key);
        }
      }
      else
      {
        Log::Error("No meta file header specified: ", filepath);
      }
    }
  }
}
