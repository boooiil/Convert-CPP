#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../../utils/logging/Logger.h"
#include "../audio/BaseAudioCodec.h"
#include "../subtitle/BaseSubtitleCodec.h"
#include "../video/BaseVideoCodec.h"

/**
 * Thoughts for container:
 *
 *  Conatiner
 *    - Check if settings support the codecs in this container.
 *    - Audio Codec
 *      - Get supported channels      [x]
 *      - Get supported bit depth     [x]
 *      - Get supported sample rates  [x]
 *    - Video Codec
 *      - Get supported resolutions   [x]
 *      - Get supported levels        [x]
 *      - Get supported tunes         [x]
 *    - Subtitle Codec
 *      - ??
 */

class BaseContainer {
public:
  // BaseContainer(BaseAudioCodec* _audio_codec,
  //               BaseSubtitleCodec* _subtitle_codec,
  //               BaseVideoCodec* _video_codec)
  //     : audio_codec(_audio_codec),
  //       subtitle_codec(_subtitle_codec),
  //       video_codec(_video_codec) {}

  virtual ~BaseContainer() {
    LOG_DEBUG("Deconstructing...");
    if (!audio_codec.empty()) {
      LOG_DEBUG("Deleting Audio Codec.");
      for (auto codec : audio_codec) {
        delete codec;
      }
      audio_codec.clear();
    }
    if (!subtitle_codec.empty()) {
      LOG_DEBUG("Deleting Subtitle Codec.");
      for (auto codec : subtitle_codec) {
        delete codec;
      }
      subtitle_codec.clear();
    }
    if (subtitle_codec_text != nullptr) {
      LOG_DEBUG("Deleting Subtitle Codec.");
      delete subtitle_codec_text;
    }
    if (video_codec != nullptr) {
      LOG_DEBUG("Deleting Video Codec.");
      delete video_codec;
    }
  };

  virtual std::string getName() = 0;
  virtual std::string getDisplayName() = 0;
  virtual std::set<std::string> getAliases() = 0;

  virtual std::vector<std::string> supportedVideoCodecs() = 0;
  virtual std::string fallbackVideoCodec() = 0;

  virtual std::vector<std::string> supportedAudioCodecs() = 0;
  virtual std::string fallbackAudioCodec() = 0;

  virtual std::vector<std::string> supportedSubtitleCodecs() = 0;
  virtual std::string fallbackSubtitleCodec() = 0;

  virtual const std::string getVideoCodec(std::string encoder) {
    return getParam(encoder, &BaseContainer::supportedVideoCodecs,
                    &BaseContainer::fallbackVideoCodec);
  }

  virtual const std::string getAudioCodec(std::string _audio_codec) {
    return getParam(_audio_codec, &BaseContainer::supportedAudioCodecs,
                    &BaseContainer::fallbackAudioCodec);
  }

  virtual const std::string getSubtitleCodec(std::string _subtitle_codec) {
    return getParam(_subtitle_codec, &BaseContainer::supportedSubtitleCodecs,
                    &BaseContainer::fallbackSubtitleCodec);
  }

  virtual const void setVideoCodec(BaseVideoCodec *_video_codec) {
    // use default video codec if null
    if (_video_codec == nullptr) {
      return;
    }

    std::string wanted_type = _video_codec->getName();
    std::string validated = getVideoCodec(wanted_type);

    // throw on invalid video codec
    if (validated != wanted_type) {
      LOG_DEBUG("Invalid video codec for container", this->getName(),
                _video_codec->getName());
      throw std::runtime_error("Invalid video codec: " +
                               _video_codec->getName());
    }

    LOG_DEBUG("Setting video codec to", _video_codec->getName());
    delete video_codec;
    video_codec = _video_codec;
  }

  virtual const void
  setAudioCodec(std::vector<BaseAudioCodec *> _audio_codecs) {
    // use default video codec if null
    if (_audio_codecs.empty()) {
      return;
    }

    for (auto _audio_codec : _audio_codecs) {
      if (_audio_codec == nullptr) {
        throw std::runtime_error("Null audio codec provided");
      }

      std::string wanted_type = _audio_codec->getName();
      std::string validated = getAudioCodec(wanted_type);

      // throw on invalid audio codec
      if (validated != wanted_type) {
        LOG_DEBUG("Invalid audio codec for container", this->getName(),
                  _audio_codec->getName());
        throw std::runtime_error("Invalid audio codec: " +
                                 _audio_codec->getName());
      }

      LOG_DEBUG("Setting audio codec to", _audio_codec->getName());
    }

    for (auto codec : audio_codec) {
      delete codec;
    }

    audio_codec = _audio_codecs;
  }

  virtual const void
  setSubtitleCodec(std::vector<BaseSubtitleCodec *> _subtitle_codecs) {
    // use default video codec if null
    if (_subtitle_codecs.empty()) {
      return;
    }

    for (auto _subtitle_codec : _subtitle_codecs) {
      if (_subtitle_codec == nullptr) {
        throw std::runtime_error("Null subtitle codec provided.");
      }

      std::string wanted_type = _subtitle_codec->getName();
      std::string validated = getSubtitleCodec(wanted_type);

      if (validated != wanted_type) {
        LOG_DEBUG("Invalid subtitle codec for container", this->getName(),
                  _subtitle_codec->getName());
        throw std::runtime_error("Invalid subtitle codec: " +
                                 _subtitle_codec->getName());
      }

      LOG_DEBUG("Setting subtitle codec to", _subtitle_codec->getName());
    }

    for (auto codec : subtitle_codec) {
      delete codec;
    }

    this->subtitle_codec = _subtitle_codecs;
  }

  auto getAudioCodecs() -> std::vector<BaseAudioCodec *> & {
    return audio_codec;
  }

  auto getVideoCodec() -> BaseVideoCodec * { return video_codec; }

  auto getSubtitleCodecs() -> std::vector<BaseSubtitleCodec *> & {
    return subtitle_codec;
  }

private:
  template <typename T, typename C>
  const T getParam(T search, std::vector<T> (C::*provider)(),
                   T (C::*fallback)()) {
    auto params = (this->*provider)();

    for (auto &p : params) {
      if (p == search) {
        return p;
      }
    }
    return (this->*fallback)();
  }

  std::vector<BaseAudioCodec *> audio_codec = {};
  std::vector<BaseSubtitleCodec *> subtitle_codec = {};
  BaseVideoCodec *video_codec = nullptr;
  BaseSubtitleCodec *subtitle_codec_text = nullptr;
};

#endif // BASE_CONTAINER_H