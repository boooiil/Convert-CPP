#ifndef CONTAINER_MKV_H
#define CONTAINER_MKV_H

#include "../../../settings/enums/AudioCodec.h"
#include "BaseContainer.h"

// TODO: turn all of these into smart pointers so i dont have to delete them

class Container_MKV : public BaseContainer {
public:
  auto getType() -> Container override { return Container::MKV; }
  auto getName() -> std::string override { return "mkv"; }

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

  auto fallbackAudioCodec() -> AudioCodec override {
    return AudioCodec::VORBIS;
  };

  auto supportedSubtitleCodecs() -> std::vector<SubtitleCodec> override {
    return {SubtitleCodec::ASS,      SubtitleCodec::DVB_SUB,
            SubtitleCodec::MOV_TEXT, SubtitleCodec::SRT,
            SubtitleCodec::SSA,      SubtitleCodec::SUBRIP,
            SubtitleCodec::WEBVTT};
  }

  auto fallbackSubtitleCodec() -> SubtitleCodec override {
    return SubtitleCodec::ASS;
  };

  auto supportedVideoCodecs() -> std::vector<Encoders> override {
    return {
        Encoders::AV1,        Encoders::AV1_AMF,    Encoders::AV1_NVENC,
        Encoders::AV1_QSV,    Encoders::H264,       Encoders::H264_AMF,
        Encoders::H264_NVENC, Encoders::H264_QSV,   Encoders::HEVC,
        Encoders::HEVC_AMF,   Encoders::HEVC_NVENC, Encoders::HEVC_QSV,
    };
  }

  auto fallbackVideoCodec() -> Encoders override { return Encoders::HEVC; };
};

#endif // CONTAINER_MKV_H