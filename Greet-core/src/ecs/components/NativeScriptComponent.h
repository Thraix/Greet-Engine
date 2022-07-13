#pragma once

#include <scripting/NativeScriptHandler.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct NativeScriptComponent
  {
    Ref<NativeScriptHandler> script;
    std::string scriptPath; // Stored to be able to be serialized, might look for a better solution at some point
    bool created = false;

    NativeScriptComponent(const Ref<NativeScriptHandler>& script);
    NativeScriptComponent(const MetaFileClass& metaClass);

    void BindEntity(const Entity& entity);
    void Create();
    void Update(float timeElapsed);
    void OnEvent(Event& event);
    void Destroy();

    friend MetaFile& operator<<(MetaFile& metaFile, const NativeScriptComponent& component);
  };
}
