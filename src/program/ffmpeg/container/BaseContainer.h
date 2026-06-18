#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/program/ffmpeg/attachment/BaseAttachment.h"
#include "src/program/ffmpeg/audio/BaseAudioCodec.h"
#include "src/program/ffmpeg/subtitle/BaseSubtitleCodec.h"
#include "src/program/ffmpeg/video/BaseVideoCodec.h"
#include "src/utils/logging/Logger.h"

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
    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing...");
    if (!audio_codec.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Deconstructing Audio Codec.");
      for (auto codec : audio_codec) {
        delete codec;
      }
      audio_codec.clear();
    }
    if (!subtitle_codec.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Deconstructing Subtitle Codec.");
      for (auto codec : subtitle_codec) {
        delete codec;
      }
      subtitle_codec.clear();
    }
    if (video_codec != nullptr) {
      LOG_DEBUG(Logger::Priority::INFO, "Deconstructing Video Codec.");
      delete video_codec;
    }
  };

  virtual const std::string getName() const = 0;
  virtual const std::string getDisplayName() const = 0;
  virtual const std::set<std::string> getAliases() const = 0;

  virtual const std::vector<std::string> supportedVideoCodecs() const = 0;
  virtual const std::string fallbackVideoCodec() const = 0;

  virtual const std::vector<std::string> supportedAudioCodecs() const = 0;
  virtual const std::string fallbackAudioCodec() const = 0;

  virtual const std::vector<std::string> supportedSubtitleCodecs() const = 0;
  virtual const std::string fallbackSubtitleCodec() const = 0;

  virtual const std::vector<std::string> supportedAttachments() const = 0;

  virtual const std::string getVideoCodec(const std::string &encoder) const {
    return getParam(encoder, &BaseContainer::supportedVideoCodecs,
                    &BaseContainer::fallbackVideoCodec);
  }

  virtual const std::string
  getAudioCodec(const std::string &_audio_codec) const {
    return getParam(_audio_codec, &BaseContainer::supportedAudioCodecs,
                    &BaseContainer::fallbackAudioCodec);
  }

  virtual const std::string
  getSubtitleCodec(const std::string &_subtitle_codec) const {
    return getParam(_subtitle_codec, &BaseContainer::supportedSubtitleCodecs,
                    &BaseContainer::fallbackSubtitleCodec);
  }

  virtual void setVideoCodec(BaseVideoCodec *_video_codec) {
    // use default video codec if null
    if (_video_codec == nullptr) {
      return;
    }

    std::string wanted_type = _video_codec->getName();
    std::string validated = getVideoCodec(wanted_type);

    // throw on invalid video codec
    if (validated != wanted_type) {
      LOG_DEBUG(Logger::Priority::INFO, "Invalid video codec for container",
                this->getName(), _video_codec->getName());
      throw std::runtime_error("Invalid video codec: " +
                               _video_codec->getName());
    }

    LOG_DEBUG(Logger::Priority::INFO, "Setting video codec to",
              _video_codec->getName());
    delete this->video_codec;
    this->video_codec = std::move(_video_codec);
  }

  virtual void
  setAudioCodec(const std::vector<BaseAudioCodec *> &_audio_codecs) {
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
        LOG_DEBUG(Logger::Priority::INFO, "Invalid audio codec for container",
                  this->getName(), _audio_codec->getName());
        throw std::runtime_error("Invalid audio codec: " +
                                 _audio_codec->getName());
      }

      LOG_DEBUG(Logger::Priority::INFO, "Setting audio codec to",
                _audio_codec->getName());
    }

    for (auto codec : audio_codec) {
      delete codec;
    }

    audio_codec = _audio_codecs;
  }

  virtual void
  setSubtitleCodec(const std::vector<BaseSubtitleCodec *> &_subtitle_codecs) {
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
        LOG_DEBUG(Logger::Priority::INFO,
                  "Invalid subtitle codec for container", this->getName(),
                  _subtitle_codec->getName());
        throw std::runtime_error("Invalid subtitle codec: " +
                                 _subtitle_codec->getName());
      }

      LOG_DEBUG(Logger::Priority::INFO, "Setting subtitle codec to",
                _subtitle_codec->getName());
    }

    for (auto codec : subtitle_codec) {
      delete codec;
    }

    this->subtitle_codec = _subtitle_codecs;
  }

  virtual void
  setAttachments(const std::vector<BaseAttachment *> &_attachments) {
    if (_attachments.empty()) {
      return;
    }

    std::vector<BaseAttachment *> valid_att = {};
    int running = 0;

    for (int i = 0; i < _attachments.size(); i++) {
      BaseAttachment *att = _attachments[i];

      if (att == nullptr) {
        throw std::runtime_error("Null attachment provided.");
      }

      std::string wanted_type = att->getName();
      std::vector<std::string> supported_attachments = supportedAttachments();

      if (std::find(supported_attachments.begin(), supported_attachments.end(),
                    wanted_type) == supported_attachments.end()) {
        LOG_DEBUG(Logger::Priority::INFO, "Invalid attachment for container",
                  this->getName(), att->getName() + ",", "dropping.");
        delete att;
        continue;
        // throw std::runtime_error("Invalid attachment: " + att->getName());
      }

      LOG_DEBUG(Logger::Priority::INFO, "Setting attachment to",
                att->getName());
      att->setIndex(i);
      att->setMapIndex(running++);
      valid_att.push_back(att);
    }

    for (auto attachment : attachments) {
      delete attachment;
    }

    this->attachments = valid_att;
  }

  auto getAudioCodecs() -> const std::vector<BaseAudioCodec *> & {
    return audio_codec;
  }

  auto getVideoCodec() -> const BaseVideoCodec * { return video_codec; }

  auto getSubtitleCodecs() -> const std::vector<BaseSubtitleCodec *> & {
    return subtitle_codec;
  }

  auto getAttachments() -> const std::vector<BaseAttachment *> & {
    return attachments;
  }

private:
  template <typename T, typename C>
  T getParam(const T &search, const std::vector<T> (C::*provider)() const,
             const T (C::*fallback)() const) const {
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
  std::vector<BaseAttachment *> attachments = {};
  BaseVideoCodec *video_codec = nullptr;
};

#endif // BASE_CONTAINER_H