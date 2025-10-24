#ifndef VIDEO_CODEC_FACTORY_H
#define VIDEO_CODEC_FACTORY_H

#include "BaseVideoCodec.h"

#include <functional>
#include <string>
#include <unordered_map>

class VideoCodecFactory {
public:
  static auto create(const std::string &name) -> BaseVideoCodec *;

private:
  static auto initialize() -> void;
  static std::unordered_map<std::string, std::function<BaseVideoCodec *()>>
      codec_map;
};

#endif // VIDEO_CODEC_FACTORY_H
