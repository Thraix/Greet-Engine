#include "NativeScriptComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  NativeScriptComponent::NativeScriptComponent(const Ref<NativeScriptHandler>& script)
    : script{script}
  {}

  NativeScriptComponent::NativeScriptComponent(const MetaFileClass& metaClass)
    : script{Ref<NativeScriptHandler>(new NativeScriptHandler{MetaFileLoading::LoadString(metaClass, "path", "")})}
  {}

  void NativeScriptComponent::BindEntity(const Entity& entity)
  {
    script->BindEntity(entity);
  }

  void NativeScriptComponent::Create()
  {
    script->OnCreate();
    created = true;
  }

  void NativeScriptComponent::Update(float timeElapsed)
  {
    if(created)
      script->OnUpdate(timeElapsed);
  }

  void NativeScriptComponent::OnEvent(Event& event)
  {
    if(created) script->OnEvent(event);
  }

  void NativeScriptComponent::Destroy()
  {
    script->OnDestroy();
    created = false;
  }
}
