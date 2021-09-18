#pragma once

#include <graphics/Color.h>
#include <graphics/fonts/Font.h>
#include <graphics/gui/TLBR.h>
#include <utils/xml/XMLObject.h>

namespace Greet {

  struct GUISize
  {
    enum class Type
    {
      Wrap, Weight, Pixels
    };

    float size = 0;
    float value = 1;
    Type type = Type::Wrap;

    GUISize()
    {}

    GUISize(float size, float value, Type type)
      : size{size}, value{value}, type{type}
    {}

    GUISize(const GUISize& size)
      : value{size.value}, type{size.type}
    {}

    GUISize(GUISize&& size)
      : value{size.value}, type{size.type}
    {}

    GUISize& operator=(const GUISize& size)
    {
      value = size.value;
      type = size.type;
      return *this;
    }

    GUISize& operator=(GUISize&& size)
    {
      value = size.value;
      type = size.type;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const GUISize& size)
    {
      if(size.type == Type::Wrap)
        return stream << "Wrap";
      else if(size.type == Type::Weight)
        return stream << "Weight: " << size.value;
      else
        return stream << "Pixel: " << size.value;
    }
  };

  class GUIUtils
  {
    public:

      static bool GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue);
      static Color GetColorFromXML(const XMLObject& object, const std::string& key, const Color& defaultValue);
      static GUISize GetGUISizeFromXML(const XMLObject& object, const std::string& key, const GUISize& defaultValue);
      static std::string GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue);
      static int GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue);
      static float GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue);

    private:
    public:
      static GUISize GetGUISize(const std::string& str);
      static Color GetColor(const std::string& str);
      static float GetFloat(const std::string& str);
      static int GetInt(const std::string& str);
      static bool GetBoolean(const std::string& str);
      static Font GetFont(const std::string& str);
      static TLBR GetTLBR(const std::string& str);
  };
}
