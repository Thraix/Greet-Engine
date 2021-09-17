#include "ComponentFactory.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Frame.h>
#include <utils/xml/XMLException.h>

namespace Greet
{
  ComponentFactory::ComponentMap* ComponentFactory::guiComponents;
  FrameFactory::FrameMap* FrameFactory::guiFrames;

  Component* ComponentFactory::GetComponent(const std::string& xmlFile, Component* parent)
  {
    ComponentMap* map = GetMap();
    try
    {
      XMLObject object = XML::FromFile(xmlFile);
      auto it = map->find(object.GetName());
      if(it == map->end())
      {
        Log::Warning("Component does not exist: ", object.GetName());
        XMLObject o("Failed", std::map<std::string, std::string>(), "");
        return new Component(o, parent); // Return plain content to avoid crash.
      }
      return it->second(object, parent);
    }
    catch(XMLException& e)
    {
      Log::Error(e.what());
      XMLObject o("Failed", std::map<std::string, std::string>(), "");
      return new Component(o, parent); // Return plain content to avoid crash.
    }
  }

  Component* ComponentFactory::GetComponent(const XMLObject& xmlObject, Component* parent)
  {
    ComponentMap* map = GetMap();
    auto it = map->find(xmlObject.GetName());
    if(it == map->end())
    {
        Log::Warning("Component does not exist: ", xmlObject.GetName());
      XMLObject o("Failed", std::map<std::string, std::string>(), "");
      return new Component(o,parent); // Return plain content to avoid crash.
    }
    return it->second(xmlObject, parent);
  }

  Frame* FrameFactory::GetFrame(const std::string& filePath)
  {
    try
    {
      return GetFrame(XML::FromFile(filePath));
    }
    catch(XMLException& e)
    {
      Log::Error(e.what());
      return new Frame();
    }
  }

  Frame* FrameFactory::GetFrame(const XMLObject& xmlObject)
  {
    FrameMap* map = GetMap();
    auto it = map->find(xmlObject.GetName());
    if(it == map->end())
    {
      Log::Warning("Frame does not exist: ", xmlObject.GetName());
      XMLObject o("Failed", std::map<std::string, std::string>(), "");
      return new Frame(o); // Return plain content to avoid crash.
    }
    return it->second(xmlObject);
  }
}
