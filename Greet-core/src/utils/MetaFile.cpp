#include "MetaFile.h"

#include <fstream>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
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
    ASSERT(it != values.end(), "Values does not exist in meta file: %s", key);
    return it->second;
  }

  const std::unordered_map<std::string, std::string>& MetaFileClass::GetValues() const
  {
    return values;
  }

  void MetaFileClass::AddValue(const std::string& key, const std::string& val)
  {
    values.emplace(key, val);
  }

  std::ostream& operator<<(std::ostream& stream, const MetaFileClass& file)
  {
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

  bool MetaFile::HasMetaClass(const std::string& className) const
  {
    return classes.find(className) != classes.end();
  }

  MetaFileClass& MetaFile::GetMetaClass(const std::string& className)
  {
    static MetaFileClass null;
    auto it = classes.find(className);
    if(it != classes.end())
      return it->second;
    return null;
  }

  std::optional<MetaFileClass> MetaFile::TryGetMetaClass(const std::string& className) const
  {
    auto it = classes.find(className);
    if(it != classes.end())
      return it->second;
    return std::optional<MetaFileClass>{};
  }

  const MetaFileClass& MetaFile::GetMetaClass(const std::string& className) const
  {
    static MetaFileClass null;
    auto it = classes.find(className);
    if(it != classes.end())
      return it->second;
    return null;
  }

  void MetaFile::AddMetaClass(const std::string& name, const MetaFileClass& metaClass)
  {
    classes[name] = metaClass;
  }

  std::ostream& operator<<(std::ostream& stream, const MetaFile& file)
  {
    for(auto metaClass : file.classes)
    {
      stream << "[" << metaClass.first << "]" << std::endl;
      stream << metaClass.second;
    }
    return stream;
  }

  std::istream& operator>>(std::istream& stream, MetaFile& file)
  {
    file.classes.clear();
    file.LoadMetaFile(stream);
    return stream;
  }

  void MetaFile::LoadMetaFile(std::istream& stream)
  {
    std::string currentClass = "";
    auto metaClassIt = classes.end();
    std::string line;
    while(std::getline(stream, line))
    {
      std::string_view trimmedLine = StringUtils::Trim(line);
      if(trimmedLine.empty())
        continue;

      if(trimmedLine == "---")
      {
        return;
      }

      if(line[StringUtils::GetTrimStartPos(line)] == '[' && line[StringUtils::GetTrimEndPos(line)] == ']' )
      {
        currentClass = StringUtils::Trim(line);
        currentClass = currentClass.substr(1, currentClass.size() - 2);
        metaClassIt = classes.find(currentClass);
        if(metaClassIt == classes.end())
          metaClassIt = classes.emplace(currentClass, MetaFileClass{}).first;
        else
          Log::Error("Meta files contains two of the same classes: %s", currentClass);
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
        auto res = metaClassIt->second.values.emplace(key, value);
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

  std::vector<MetaFile> MetaFile::ReadList(const std::string& file)
  {
    std::vector<MetaFile> metaFiles;
    std::ifstream stream{file};
    if(stream)
    {
      MetaFile meta;
      while(!stream.eof())
      {
        MetaFile meta{};
        stream >> meta;
        if(meta.classes.empty())
          continue;
        metaFiles.emplace_back(meta);
      }
      return metaFiles;
    }
    return {};
  }
}
