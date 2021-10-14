#pragma once

#include <graphics/Color.h>
#include <graphics/fonts/Font.h>
#include <graphics/gui/StyleTemplateClasses.h>
#include <graphics/gui/TLBR.h>
#include <utils/MetaFile.h>

#include <unordered_map>

#define REGISTER_STYLE_FUNCTIONS(ret, name, map) \
  bool Has##name(const std::string& str) \
  {\
    return map.HasValue(str);\
  }\
  \
  const ret& Get##name(const std::string& str) const\
  {\
    return map.GetValue(str);\
  }\
  \
  ComponentStyle& Set##name(const std::string& str, const ret& val) \
  { \
    map.SetValue(str, val); \
    return *this; \
  }

namespace Greet
{
  struct StylingVariables
  {
    StyleVariableMap<Color> colors;
    StyleVariableMap<TLBR> tlbrs;
    StyleVariableMap<float> floats;
    StyleVariableMap<int> ints;
    StyleVariableMap<bool> bools;
    StyleVariableMap<Font> fonts;
    StyleVariableMap<GUISize> sizes;

    void Merge(const StylingVariables& variables)
    {
      colors.Merge(variables.colors);
      tlbrs.Merge(variables.tlbrs);
      floats.Merge(variables.floats);
      ints.Merge(variables.ints);
      bools.Merge(variables.bools);
      fonts.Merge(variables.fonts);
      sizes.Merge(variables.sizes);
    }
  };

  struct Styling
  {
    std::unordered_map<std::string, Color> colors;
    std::unordered_map<std::string, TLBR> tlbrs;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, Font> fonts;
    std::unordered_map<std::string, GUISize> sizes;
  };

  struct ComponentStyle
  {
    private:
      ComponentStyle* inherit;

      StyleMap<Color, ColorStyle> colors;
      StyleMap<TLBR, TLBRStyle> tlbrs;
      StyleMap<float, FloatStyle> floats;
      StyleMap<int, IntStyle> ints;
      StyleMap<bool, BoolStyle> bools;
      StyleMap<Font, FontStyle> fonts;
      StyleMap<GUISize, GUISizeStyle> sizes;
    public:

      ComponentStyle()
      {}

      ComponentStyle(const StylingVariables& variables, ComponentStyle* inherit)
      {
        SetInherit(inherit);
        AddVariables(variables);
      }

      void SetInherit(ComponentStyle* aInherit)
      {
        inherit = aInherit;
        if(inherit != nullptr)
        {
          colors.SetInherit(&inherit->colors);
          tlbrs.SetInherit(&inherit->tlbrs);
          floats.SetInherit(&inherit->floats);
          ints.SetInherit(&inherit->ints);
          bools.SetInherit(&inherit->bools);
          fonts.SetInherit(&inherit->fonts);
          sizes.SetInherit(&inherit->sizes);
        }
      }

      void Load(const Styling& styling)
      {
        colors.Load(styling.colors);
        tlbrs.Load(styling.tlbrs);
        floats.Load(styling.floats);
        ints.Load(styling.ints);
        bools.Load(styling.bools);
        fonts.Load(styling.fonts);
        sizes.Load(styling.sizes);
      }

      void Load(const std::string& mode, const XMLObject& object)
      {
        colors.Load(mode, object);
        tlbrs.Load(mode, object);
        floats.Load(mode, object);
        ints.Load(mode, object);
        bools.Load(mode, object);
        fonts.Load(mode, object);
        sizes.Load(mode, object);
      }

      void Load(const std::string& componentType, const std::string& componentName, const std::string& mode, const MetaFile& object)
      {
        auto classesType = object.GetMetaClass(componentType);
        MetaFileClass* stylingType = nullptr;
        if(classesType.size() != 0)
        {
          stylingType = &classesType[0];
          if(classesType.size() > 1)
            Log::Warning("More than one style specified in style file for component \"%s\"", classesType);
        }

        auto classesName = object.GetMetaClass(componentName);
        MetaFileClass* stylingName = nullptr;
        if(classesName.size() != 0)
        {
          stylingName = &classesName[0];
          if(classesName.size() > 1)
            Log::Warning("More than one style specified in style file for component \"%s\"", classesName);
        }

        auto classesVariables = object.GetMetaClass("Variables");
        MetaFileClass* stylingVariables = nullptr;
        if(classesVariables.size() != 0)
        {
          stylingVariables = &classesVariables[0];
          if(classesVariables.size() > 1)
            Log::Warning("More than one variable meta class specified in style file for component \"%s\"", stylingVariables);
        }

        colors.Load(mode, stylingType, stylingName, stylingVariables);
        tlbrs.Load(mode, stylingType, stylingName, stylingVariables);
        floats.Load(mode, stylingType, stylingName, stylingVariables);
        ints.Load(mode, stylingType, stylingName, stylingVariables);
        bools.Load(mode, stylingType, stylingName, stylingVariables);
        fonts.Load(mode, stylingType, stylingName, stylingVariables);
        sizes.Load(mode, stylingType, stylingName, stylingVariables);
      }

      void AddVariables(const StylingVariables& style)
      {
        colors.AddVariables(style.colors);
        tlbrs.AddVariables(style.tlbrs);
        floats.AddVariables(style.floats);
        ints.AddVariables(style.ints);
        bools.AddVariables(style.bools);
        fonts.AddVariables(style.fonts);
        sizes.AddVariables(style.sizes);
      }

      void SetVariables(const StylingVariables& style)
      {
        colors.SetVariables(style.colors);
        tlbrs.SetVariables(style.tlbrs);
        floats.SetVariables(style.floats);
        ints.SetVariables(style.ints);
        bools.SetVariables(style.bools);
        fonts.SetVariables(style.fonts);
        sizes.SetVariables(style.sizes);
      }

      REGISTER_STYLE_FUNCTIONS(Color, Color, colors);
      REGISTER_STYLE_FUNCTIONS(TLBR, TLBR, tlbrs);
      REGISTER_STYLE_FUNCTIONS(float, Float, floats);
      REGISTER_STYLE_FUNCTIONS(int, Int, ints);
      REGISTER_STYLE_FUNCTIONS(bool, Bool, bools);
      REGISTER_STYLE_FUNCTIONS(Font, Font, fonts);
      REGISTER_STYLE_FUNCTIONS(GUISize, GUISize, sizes);

      void SetStyling()
      {
        colors.SetStyle();
        tlbrs.SetStyle();
        floats.SetStyle();
        ints.SetStyle();
        bools.SetStyle();
        fonts.SetStyle();
        sizes.SetStyle();
      }
  };
}
