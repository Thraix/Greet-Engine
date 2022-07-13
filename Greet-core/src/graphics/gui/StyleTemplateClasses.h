#pragma once

#include <graphics/fonts/Font.h>
#include <graphics/gui/GUIUtils.h>
#include <graphics/gui/TLBR.h>
#include <logging/Log.h>
#include <utils/MetaFile.h>

#include <string>
#include <unordered_map>

namespace Greet
{
  template<typename T>
  struct StyleVariable
  {
    bool mbSetVariable = false; // if false the value will be set to the inherited value
    T mVariable;
    T* mpVariable;

    StyleVariable(bool abSetVariable, T* apVariable)
      : mbSetVariable{abSetVariable}, mVariable{*apVariable}, mpVariable{apVariable}
    {}
  };

  template <typename T>
  struct StyleVariableMap
  {
    std::unordered_map<std::string, T*> values;

    StyleVariableMap()
    {}

    StyleVariableMap(const std::initializer_list<std::pair<std::string, T*>>& values)
      : values{values.begin(), values.end()}
    {}

    void Merge(const StyleVariableMap& avValues)
    {
      for(auto&& value : avValues.values)
      {
        auto it = values.find(value.first);
        if(it != values.end())
        {
          Log::Error("Styling already exist for component: ", value.first);
          continue;
        }
        values.emplace(value.first, value.second);
      }
    }
  };

  template <typename T, typename StrToValFunc>
  struct StyleMap
  {
    std::unordered_map<std::string, StyleVariable<T>> values;
    StyleMap* inherit = nullptr;

    void SetInherit(StyleMap* aInherit)
    {
      inherit = aInherit;
    }

    void Load(const std::unordered_map<std::string, T>& avValues)
    {
      for(auto&& value : avValues)
      {
        SetValue(value.first, value.second);
      }
    }

    void Load(const std::string& mode, const XMLObject& object)
    {
      for(auto&& value : values)
      {
        std::string attribute = GetAttributeName(mode, value.first);
        if(object.HasAttribute(attribute))
          SetValue(value.first, StrToValFunc::Get(object.GetAttribute(attribute)));
      }
    }

    void Load(const std::string& mode, const std::optional<MetaFileClass>& stylingType, const std::optional<MetaFileClass>& stylingName, const std::optional<MetaFileClass>& stylingVariables)
    {
      for(auto&& value : values)
      {
        if(!value.second.mbSetVariable)
          LoadStylingVariable(mode, value.first, stylingName, stylingVariables);
        if(!value.second.mbSetVariable)
          LoadStylingVariable(mode, value.first, stylingType, stylingVariables);
      }
    }


    void AddVariables(const StyleVariableMap<T>& avValues)
    {
      for(auto&& value : avValues.values)
      {
        auto it = values.find(value.first);
        if(it != values.end())
        {
          Log::Error("Styling already exist for component: ", value.first);
          continue;
        }
        values.emplace(value.first, StyleVariable{false, value.second});
      }
    }

    void SetVariables(const StyleVariableMap<T>& avValues)
    {
      values.clear();
      for(auto&& value : avValues.values)
      {
        values.emplace(value.first, StyleVariable{false, value.second});
      }
    }

    bool HasValue(const std::string& key)
    {
      return values.find(key) != values.end();
    }

    const T& GetValue(const std::string& key) const
    {
      auto it = values.find(key);
      ASSERT(it != values.end(), "Component does not contain styling: ", key);
      if(!it->second.mbSetVariable && inherit)
        return inherit->GetValue(key);
      return it->second.mVariable;
    }

    StyleMap& SetValue(const std::string& key, const T& value)
    {
      auto it = values.find(key);
      if(it == values.end())
      {
        Log::Error("Component doesn't contain styling: ", key);
        return *this;
      }
      it->second.mbSetVariable = true;
      it->second.mVariable  = value;
      *it->second.mpVariable = value;
      return *this;
    }

    void SetStyle()
    {
      for(auto&& value : values)
      {
        *value.second.mpVariable = GetValue(value.first);
      }
    }

    private:
      void LoadStylingVariable(const std::string& mode, const std::string& variable, const std::optional<MetaFileClass>& styling, const std::optional<MetaFileClass>& stylingVariables)
      {
        std::string attribute = GetAttributeName(mode, variable);
        if(styling)
        {
          if(styling->HasValue(attribute))
          {
            const std::string& str = styling->GetValue(attribute);
            if(str[0] == '$')
            {
              if(!stylingVariables->HasValue(str.substr(1)))
              {
                Log::Warning("Styling variable does not exist: ", str.substr(1));
              }
              else
              {
                const std::string& val = stylingVariables->GetValue(str.substr(1));
                SetValue(variable, StrToValFunc::Get(val));
              }
            }
            else
            {
              SetValue(variable, StrToValFunc::Get(styling->GetValue(attribute)));
            }
          }
        }
      }

      std::string GetAttributeName(const std::string& mode, const std::string& variable)
      {
        return mode == "normal" ? variable : mode + "-" + variable;
      }
  };

  struct ColorStyle
  {
    static Color Get(const std::string& str) { return GUIUtils::GetColor(str); }
  };

  struct TLBRStyle
  {
    static TLBR Get(const std::string& str) { return GUIUtils::GetTLBR(str); }
  };

  struct FloatStyle
  {
    static float Get(const std::string& str) { return GUIUtils::GetFloat(str); }
  };

  struct IntStyle
  {
    static int Get(const std::string& str) { return GUIUtils::GetInt(str); }
  };

  struct BoolStyle
  {
    static bool Get(const std::string& str) { return GUIUtils::GetBoolean(str); }
  };

  struct FontStyle
  {
    static Font Get(const std::string& str) { return GUIUtils::GetFont(str); }
  };

  struct GUISizeStyle
  {
    static GUISize Get(const std::string& str) { return GUIUtils::GetGUISize(str); }
  };
}
