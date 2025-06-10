#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H

#include <vector>

#include "../../../../utils/logging/Logger.h"
#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/Container.h"
#include "../../../settings/enums/Encoders.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
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
    // LOG_DEBUG("Deconstructing...");
    // if (audio_codec != nullptr) {
    //   LOG_DEBUG("Deleting Audio Codec.");
    //   delete audio_codec;
    // }
    // if (subtitle_codec != nullptr) {
    //   LOG_DEBUG("Deleting Subtitle Codec.");
    //   delete subtitle_codec;
    // }
    // if (video_codec != nullptr) {
    //   LOG_DEBUG("Deleting Video Codec.");
    //   delete video_codec;
    // }
  };

  virtual Container getType() = 0;

  virtual std::vector<Encoders> supportedVideoCodecs() = 0;
  virtual Encoders fallbackVideoCodec() = 0;

  virtual const Encoders getVideoCodec(Encoders encoder) {
    return getParam(encoder, &BaseContainer::supportedVideoCodecs,
                    &BaseContainer::fallbackVideoCodec);
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

  // BaseAudioCodec* audio_codec = nullptr;
  // BaseVideoCodec* video_codec = nullptr;
  // BaseSubtitleCodec* subtitle_codec = nullptr;
};

#endif  // BASE_CONTAINER_H