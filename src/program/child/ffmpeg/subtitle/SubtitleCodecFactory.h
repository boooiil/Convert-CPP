#ifndef SUBTITLE_CODEC_FACTORY_H
#define SUBTITLE_CODEC_FACTORY_H

#include "BaseSubtitleCodec.h"

#include <functional>
#include <string>
#include <unordered_map>

class SubtitleCodecFactory {
public:
  static auto create(const std::string &name) -> BaseSubtitleCodec *;

private:
  static auto initialize() -> void;
  static std::unordered_map<std::string, std::function<BaseSubtitleCodec *()>>
      codec_map;
};

#endif // SUBTITLE_CODEC_FACTORY_H
