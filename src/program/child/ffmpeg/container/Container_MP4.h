#ifndef CONTAINER_MP4_H
#define CONTAINER_MP4_H

#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../audio/AudioCodec_AAC.h"
#include "../audio/AudioCodec_AC3.h"
#include "../audio/AudioCodec_FLAC.h"
#include "../audio/AudioCodec_MP3.h"
#include "../audio/AudioCodec_OPUS.h"
#include "../audio/AudioCodec_VORBIS.h"
#include "BaseContainer.h"

// TODO: turn all of these into smart pointers so i dont have to delete them

class Container_MP4 : public BaseContainer {
 public:
  auto getType() -> Container override { return Container::MP4; }
  auto getName() -> std::string override { return "mp4"; }

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
    return {Encoders::H264, Encoders::H264_AMF, Encoders::H264_QSV};
  }

  auto fallbackVideoCodec() -> Encoders override { return Encoders::H264; };
};

#endif  // CONTAINER_MP4_H