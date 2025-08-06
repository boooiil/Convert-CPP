#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H

#include <vector>

#include "../../../../utils/logging/Logger.h"
#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/Container.h"
#include "../../../settings/enums/Encoders.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "../audio/AudioCodec_AAC.h"
#include "../audio/BaseAudioCodec.h"
#include "../subtitle/BaseSubtitleCodec.h"
#include "../subtitle/SubtitleCodec_ASS.h"
#include "../video/BaseVideoCodec.h"
#include "../video/VideoCodec_HEVC.h"

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
    if (subtitle_codec != nullptr) {
      LOG_DEBUG("Deleting Subtitle Codec.");
      delete subtitle_codec;
    }
    if (video_codec != nullptr) {
      LOG_DEBUG("Deleting Video Codec.");
      delete video_codec;
    }
  };

  virtual Container getType() = 0;
  virtual std::string getName() = 0;

  virtual std::vector<Encoders> supportedVideoCodecs() = 0;
  virtual Encoders fallbackVideoCodec() = 0;

  virtual const Encoders getVideoCodec(Encoders encoder) {
    return getParam(encoder, &BaseContainer::supportedVideoCodecs,
                    &BaseContainer::fallbackVideoCodec);
  }
  virtual const void setVideoCodec(BaseVideoCodec* _video_codec) {
    // use default video codec if null
    if (_video_codec == nullptr) {
      return;
    }

    Encoders wanted_type = _video_codec->getType();
    Encoders validated = getVideoCodec(wanted_type);

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

  /**
   * @brief List of supported audio codecs supported by this video codec.
   *
   * @return std::vector<Encoders>
   */
  virtual std::vector<AudioCodec> supportedAudioCodecs() = 0;

  /**
   * @brief The fallback audio codec to use if the primary one is not available.
   *
   * @return Encoders
   */
  virtual AudioCodec fallbackAudioCodec() = 0;

  virtual const AudioCodec getAudioCodec(AudioCodec _audio_codec) {
    return getParam(_audio_codec, &BaseContainer::supportedAudioCodecs,
                    &BaseContainer::fallbackAudioCodec);
  }
  virtual const void setAudioCodec(std::vector<BaseAudioCodec*> _audio_codecs) {
    // use default video codec if null
    if (_audio_codecs.empty()) {
      return;
    }

    for (auto _audio_codec : _audio_codecs) {
      if (_audio_codec == nullptr) {
        throw std::runtime_error("Null audio codec provided");
      }

      AudioCodec wanted_type = _audio_codec->getType();
      AudioCodec validated = getAudioCodec(wanted_type);

      // throw on invalid video codec
      if (validated != wanted_type) {
        LOG_DEBUG("Invalid audio codec for container", this->getName(),
                  _audio_codec->getName());
        throw std::runtime_error("Invalid video codec: " +
                                 _audio_codec->getName());
      }

      LOG_DEBUG("Setting video codec to", _audio_codec->getName());
    }

    for (auto codec : audio_codec) {
      delete codec;
    }

    audio_codec = _audio_codecs;
  }

  /**
   * @brief List of supported subtitle codecs supported by this video codec.
   *
   * @return std::vector<Encoders>
   */
  virtual std::vector<SubtitleCodec> supportedSubtitleCodecs() = 0;

  /**
   * @brief The fallback subtitle codec to use if the primary one is not
   * available.
   *
   * @return Encoders
   */
  virtual SubtitleCodec fallbackSubtitleCodec() = 0;

  virtual const SubtitleCodec getSubtitleCodec(SubtitleCodec _subtitle_codec) {
    return getParam(_subtitle_codec, &BaseContainer::supportedSubtitleCodecs,
                    &BaseContainer::fallbackSubtitleCodec);
  }
  virtual const void setSubtitleCodec(BaseSubtitleCodec* _subtitle_codec) {
    // use default video codec if null
    if (_subtitle_codec == nullptr) {
      return;
    }

    SubtitleCodec wanted_type = _subtitle_codec->getType();
    SubtitleCodec validated = getSubtitleCodec(wanted_type);

    // throw on invalid video codec
    if (validated != wanted_type) {
      LOG_DEBUG("Invalid audio codec for container", this->getName(),
                _subtitle_codec->getName());
      throw std::runtime_error("Invalid video codec: " +
                               _subtitle_codec->getName());
    }

    LOG_DEBUG("Setting video codec to", _subtitle_codec->getName());
    delete subtitle_codec;
    subtitle_codec = _subtitle_codec;
  }

  auto getAudioCodecs() -> const std::vector<BaseAudioCodec*>& {
    return audio_codec;
  }

  auto getVideoCodec() -> const BaseVideoCodec* { return video_codec; }

  auto getSubtitleCodec() -> const BaseSubtitleCodec* { return subtitle_codec; }

 private:
  template <typename T, typename C>
  const T getParam(T search, std::vector<T> (C::*provider)(),
                   T (C::*fallback)()) {
    auto params = (this->*provider)();

    for (auto& p : params) {
      if (p == search) {
        return p;
      }
    }
    return (this->*fallback)();
  }

  std::vector<BaseAudioCodec*> audio_codec = {};
  BaseVideoCodec* video_codec = new VideoCodec_HEVC();
  BaseSubtitleCodec* subtitle_codec = new SubtitleCodec_ASS();
};

#endif  // BASE_CONTAINER_H