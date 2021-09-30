#include "JSONArray.h"

#include <logging/Log.h>
#include <utils/json/JSON.h>
#include <utils/json/JSONObject.h>

namespace Greet {

  JSONArray::JSONArray()
  {

  }

  JSONArray::JSONArray(const JSONArray& jsonArray)
    : m_objects(jsonArray.m_objects)
  {

  }

  JSONArray::JSONArray(std::vector<JSONObject> objects)
    : m_objects(objects)
  {

  }

  void JSONArray::AddObject(JSONObject object)
  {
    m_objects.push_back(object);
  }

  const JSONObject& JSONArray::GetObject(size_t i) const
  {
    size_t size = m_objects.size();
    if (i >= size)
    {
      Log::Error("Index is out of bounds: %s >= %s", i, size);
      if (size != 0)
      {
        return GetObject(0);
      }
      static JSONObject outOfBound;
      return outOfBound;
    }
    return m_objects[i];
  }

  const JSONObject& JSONArray::operator[](size_t i) const
  {
    return GetObject(i);
  }

  std::ostream& operator<<(std::ostream& os, const JSONArray& arr)
  {
    os << JSON::GetIndent() << "[\n";
    JSON::PushIndent();
    uint32_t size = arr.m_objects.size();
    for (uint32_t i = 0;i<size;++i)
    {
      os << arr.GetObject(i);
      if(i != size - 1)
      {
        os << ",\n";
      }
    }
    JSON::PopIndent();
    os << "\n" << JSON::GetIndent() << "]";
    return os;
  }
}
