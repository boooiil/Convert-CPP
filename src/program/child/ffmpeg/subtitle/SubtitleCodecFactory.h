#ifndef SUBTITLE_CODEC_FACTORY_H
#define SUBTITLE_CODEC_FACTORY_H

#include "BaseSubtitleCodec.h"
#include "src/program/registry/Registry.h"

#include <functional>
#include <string>

class SubtitleCodecFactory {
public:
  static auto create(const std::string &name) -> BaseSubtitleCodec *;
  static auto registry()
      -> const Registry<std::string, std::function<BaseSubtitleCodec *()>> &;

private:
  static auto initialize() -> void;
  static Registry<std::string, std::function<BaseSubtitleCodec *()>>
      codec_registry;
};

#endif // SUBTITLE_CODEC_FACTORY_H
