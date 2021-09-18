#pragma once

#include <common/Memory.h>
#include <ecs/Entity.h>
#include <event/Event.h>

#define REGISTER_NATIVE_SCRIPT(SCRIPT_NAME)  extern "C" SCRIPT_NAME* New() { return new SCRIPT_NAME; }

namespace Greet
{
  struct NativeScript
  {
    DELETE_COPY_AND_MOVE(NativeScript);

    NativeScript() {}
    Entity entity{nullptr};
    virtual ~NativeScript() {}

    virtual void OnCreate() {}
    virtual void OnUpdate(float timeElapsed) {}
    virtual void OnEvent(Event& event) {}
    virtual void OnDestroy() {}

    Entity FindEntityByTag(const std::string& tag);
  };
};
