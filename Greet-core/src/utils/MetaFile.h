#pragma once

#include <map>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Greet
{
  class MetaFileClass
  {
    friend class MetaFile;
    private:
      std::unordered_map<std::string, std::string> values;
      std::string className;

      MetaFileClass(const std::string& className);

    public:
      MetaFileClass(const std::string& className, const std::unordered_map<std::string, std::string>& values)
        : className{className}, values{values}
      {}
      std::string GetValue(const std::string& key, const std::string& val) const;
      bool HasValue(const std::string& key) const;
      const std::string& GetValue(const std::string& key) const;
      const std::unordered_map<std::string, std::string>& GetValues() const;

      friend std::ostream& operator<<(std::ostream& stream, const MetaFileClass& file);
  };

  class MetaFile
  {
    private:
      std::string filepath;
      std::map<std::string, std::vector<MetaFileClass>> classes; // map<class, map<variable, value>>

    public:
      MetaFile();
      MetaFile(const std::string& filepath);
      MetaFile(std::istringstream&& stream);

      bool HasMetaClass(const std::string& className);
      std::vector<MetaFileClass>& GetMetaClass(const std::string& className);
      const std::vector<MetaFileClass>& GetMetaClass(const std::string& className) const;

      friend std::ostream& operator<<(std::ostream& stream, const MetaFile& file);
    private:
      void LoadMetaFile(std::istream& stream);
  };
}
