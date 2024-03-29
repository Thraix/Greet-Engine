#pragma once

#include <string>

namespace Greet{

  struct AudioChannel
  {
    std::string m_name;
    float m_volume;
    float m_pitch;
    float m_pan;

    AudioChannel(const std::string& name, float volume, float pitch, float pan);
    AudioChannel(const std::string& name, float volume);
    AudioChannel(const std::string& name);

    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetPan(float pan);

    inline const std::string& GetName() const { return m_name; }
    inline const float GetVolume() const  { return m_volume; }
    inline const float GetPitch() const  { return m_pitch; }
    inline const float GetPan() const  { return m_pan; }
  };
}
