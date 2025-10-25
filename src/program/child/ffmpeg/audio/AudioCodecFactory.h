#ifndef AUDIO_CODEC_FACTORY_H
#define AUDIO_CODEC_FACTORY_H

#include "BaseAudioCodec.h"
#include "src/program/registry/Registry.h"
#include <functional>
#include <string>

class AudioCodecFactory {
public:
  static auto create(const std::string &name) -> BaseAudioCodec *;
  static auto registry()
      -> const Registry<std::string, std::function<BaseAudioCodec *()>> &;

private:
  static auto initialize() -> void;
  static Registry<std::string, std::function<BaseAudioCodec *()>>
      codec_registry;
};

#endif // AUDIO_CODEC_FACTORY_H
