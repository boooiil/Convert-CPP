#ifndef AUDIO_CODEC_FACTORY_H
#define AUDIO_CODEC_FACTORY_H

#include "BaseAudioCodec.h"
#include <functional>
#include <string>
#include <unordered_map>

class AudioCodecFactory {
public:
  static auto create(const std::string &name) -> BaseAudioCodec *;

private:
  static auto initialize() -> void;
  static std::unordered_map<std::string, std::function<BaseAudioCodec *()>>
      codec_map;
};

#endif // AUDIO_CODEC_FACTORY_H
