#pragma once

#include <utils/MetaFile.h>
#include <graphics/fonts/Font.h>
#include <graphics/gui/GUIUtils.h>
#include <graphics/gui/StyleTemplateClasses.h>
#include <graphics/gui/TLBR.h>

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
  ComponentStyle& SetColor(const std::string& str, const ret& val) \
  { \
    map.SetValue(str, val); \
    return *this; \
  }

namespace Greet
{
  struct StylingVariables
  {
    StyleVariableMap<Vec4> colors;
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
    std::unordered_map<std::string, Vec4> colors;
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

      StyleMap<Vec4, ColorStyle> colors;
      StyleMap<TLBR, TLBRStyle> tlbrs;
      StyleMap<float, FloatStyle> floats;
      StyleMap<int, IntStyle> ints;
      StyleMap<bool, BoolStyle> bools;
      StyleMap<Font, FontStyle> fonts;
      StyleMap<GUISize, GUISizeStyle> sizes;
    public:

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

      void Load(const std::string& component, const std::string& mode, const MetaFile& object)
      {
        colors.Load(component, mode, object);
        tlbrs.Load(component, mode, object);
        floats.Load(component, mode, object);
        ints.Load(component, mode, object);
        bools.Load(component, mode, object);
        fonts.Load(component, mode, object);
        sizes.Load(component, mode, object);
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

      REGISTER_STYLE_FUNCTIONS(Vec4, Color, colors);
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
