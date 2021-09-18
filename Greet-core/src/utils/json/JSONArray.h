#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>

namespace Greet {

  class JSONObject;

  class JSONArray
  {
    public:
      friend class JSONObject;
    private:
      std::vector<JSONObject> m_objects;
    public:
      JSONArray();
      JSONArray(const JSONArray& jsonArray);
      JSONArray(std::vector<JSONObject> objects);
      void AddObject(JSONObject object);
      const JSONObject& GetObject(size_t i) const;
      const JSONObject& operator[](size_t i) const;
      uint32_t Size() const { return m_objects.size(); }

      friend std::ostream& operator<<(std::ostream& os, const JSONArray& dt);
  };

}
