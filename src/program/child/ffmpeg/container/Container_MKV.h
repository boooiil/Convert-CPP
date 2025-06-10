#ifndef CONTAINER_MKV_H
#define CONTAINER_MKV_H

#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "BaseContainer.h"

// TODO: turn all of these into smart pointers so i dont have to delete them

class Container_MKV : public BaseContainer {
 public:
  auto getType() -> StringEnumDataHolder<Container> override {
    return Container::MKV;
  }

  auto supportedAudioCodecs() -> std::vector<AudioCodec> override {
    return {
        AudioCodec::AAC,     // aac
        AudioCodec::AC3,     // ac3
        AudioCodec::MP3,     // mp3
        AudioCodec::FLAC,    // flac
        AudioCodec::VORBIS,  // vorbis
        AudioCodec::OPUS     // opus
    };
  }

  auto fallbackAudioCodec() -> AudioCodec override { return AudioCodec::AAC; };

  auto supportedSubtitleCodecs() -> std::vector<SubtitleCodec> override {
    return {/* TODO: add these when we finish sub codec */};
  }

  auto fallbackSubtitleCodec() -> SubtitleCodec override {
    return SubtitleCodec::ASS;
  };

  auto supportedVideoCodecs() -> std::vector<Encoders> override {
    return Encoders::_all();
  }

  auto fallbackVideoCodec() -> Encoders override { return Encoders::HEVC; };
};

#endif  // CONTAINER_MKV_H