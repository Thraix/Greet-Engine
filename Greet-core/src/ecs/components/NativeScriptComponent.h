#pragma once

#include <scripting/NativeScriptHandler.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct NativeScriptComponent
  {
    Ref<NativeScriptHandler> script;
    bool created = false;

    NativeScriptComponent(const Ref<NativeScriptHandler>& script);
    NativeScriptComponent(const MetaFileClass& metaClass);

    void BindEntity(const Entity& entity);
    void Create();
    void Update(float timeElapsed);
    void OnEvent(Event& event);
    void Destroy();
  };
}
