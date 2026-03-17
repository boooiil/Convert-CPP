#ifndef VIDEO_CODEC_FACTORY_H
#define VIDEO_CODEC_FACTORY_H

#include "BaseVideoCodec.h"
#include "src/program/registry/Registry.h"

#include <functional>
#include <string>

class VideoCodecFactory {
public:
  static auto create(const std::string &name) -> BaseVideoCodec *;
  static auto registry()
      -> const Registry<std::string, std::function<BaseVideoCodec *()>> &;

private:
  static auto initialize() -> void;
  static Registry<std::string, std::function<BaseVideoCodec *()>>
      codec_registry;
};

#endif // VIDEO_CODEC_FACTORY_H
