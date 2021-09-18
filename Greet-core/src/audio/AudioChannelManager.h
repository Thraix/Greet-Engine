#pragma once

#include <audio/AudioChannel.h>

#include <vector>

namespace Greet {
  class AudioChannelManager
  {
    private:
      static std::vector<AudioChannel*> m_channels;
    public:
      static void Add(AudioChannel* channel);
      static AudioChannel* Get(const std::string& channelname);
      static void Destroy();
    private:
      AudioChannelManager(){}
  };
}
