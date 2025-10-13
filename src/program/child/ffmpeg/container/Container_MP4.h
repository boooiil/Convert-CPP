#ifndef CONTAINER_MP4_H
#define CONTAINER_MP4_H

#include "../../../settings/enums/AudioCodec.h"
#include "BaseContainer.h"
#include "src/program/settings/enums/SubtitleCodec.h"

// TODO: turn all of these into smart pointers so i dont have to delete them

class Container_MP4 : public BaseContainer {
public:
  auto getType() -> Container override { return Container::MP4; }
  auto getName() -> std::string override { return "mp4"; }

  auto supportedAudioCodecs() -> std::vector<AudioCodec> override {
    return {
        AudioCodec::AAC,    // aac
        AudioCodec::AC3,    // ac3
        AudioCodec::MP3,    // mp3
        AudioCodec::FLAC,   // flac
        AudioCodec::VORBIS, // vorbis
        AudioCodec::OPUS    // opus
    };
  }

  auto fallbackAudioCodec() -> AudioCodec override { return AudioCodec::AAC; };

  auto supportedSubtitleCodecs() -> std::vector<SubtitleCodec> override {
    return {SubtitleCodec::DVB_SUB, SubtitleCodec::SRT};
  }

  auto fallbackSubtitleCodec() -> SubtitleCodec override {
    return SubtitleCodec::SRT;
  };

  auto supportedVideoCodecs() -> std::vector<Encoders> override {
    return {Encoders::H264,       Encoders::H264_NVENC, Encoders::H264_AMF,
            Encoders::H264_QSV,   Encoders::HEVC,       Encoders::HEVC_AMF,
            Encoders::HEVC_NVENC, Encoders::HEVC_QSV};
  }

  auto fallbackVideoCodec() -> Encoders override { return Encoders::H264; };
};

#endif // CONTAINER_MP4_H