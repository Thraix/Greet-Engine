#pragma once

#include <utils/json/JSONArray.h>
#include <utils/json/JSONObject.h>

namespace Greet {

  class JSONLoader
  {
    public:
      static JSONObject LoadJSON(const std::string& filename);
    private:

      static bool IsKeyValue(const std::string& file, uint32_t& currentPos, std::string* key, std::string* value, JSONObject* object, JSONArray* jsonArray, uint32_t* type);
      static bool IsValue(const std::string& file, uint32_t& currentPos, std::string* value, JSONObject* object, JSONArray* jsonArray, uint32_t* type);
      static bool IsString(const std::string& file, uint32_t& currentPos, std::string* value);
      static bool IsNumber(const std::string& file, uint32_t& currentPos, std::string* value);
      static bool IsObject(const std::string& file, uint32_t& currentPos, JSONObject* object);
      static bool IsArray(const std::string& file, uint32_t& currentPos, JSONArray* jsonArray);
      static void SetString(const std::string& file, uint32_t start, uint32_t end, std::string* set);
      static bool IsIndent(const std::string& file, uint32_t& currentPos);
  };

}
