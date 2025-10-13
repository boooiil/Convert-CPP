#ifndef VIDEO_CODEC_FACTORY_H
#define VIDEO_CODEC_FACTORY_H

#include "../../../settings/enums/Encoders.h"
#include "BaseVideoCodec.h"
#include "VideoCodec_AV1.h"
#include "VideoCodec_AV1_AMF.h"
#include "VideoCodec_AV1_NVENC.h"
#include "VideoCodec_AV1_QSV.h"
#include "VideoCodec_H264.h"
#include "VideoCodec_H264_AMF.h"
#include "VideoCodec_H264_NVENC.h"
#include "VideoCodec_H264_QSV.h"
#include "VideoCodec_HEVC.h"
#include "VideoCodec_HEVC_AMF.h"
#include "VideoCodec_HEVC_NVENC.h"
#include "VideoCodec_HEVC_QSV.h"
#include <functional>
#include <string>
#include <unordered_map>

class VideoCodecFactory {
public:
  static auto create(Encoders videoCodec) -> BaseVideoCodec * {
    switch (videoCodec) {
    case Encoders::AV1:
      return new VideoCodec_AV1();
    case Encoders::AV1_AMF:
      return new VideoCodec_AV1_AMF();
    case Encoders::AV1_NVENC:
      return new VideoCodec_AV1_NVENC();
      //   case Encoders::AV1_QSV:
      //     return new VideoCodec_AV1_QSV();
    case Encoders::H264:
      return new VideoCodec_H264();
    case Encoders::H264_AMF:
      return new VideoCodec_H264_AMF();
    case Encoders::H264_NVENC:
      return new VideoCodec_H264_NVENC();
      //   case Encoders::H264_QSV:
      //     return new VideoCodec_H264_QSV();
    case Encoders::HEVC:
      return new VideoCodec_HEVC();
    case Encoders::HEVC_AMF:
      return new VideoCodec_HEVC_AMF();
    case Encoders::HEVC_NVENC:
      return new VideoCodec_HEVC_NVENC();
      //   case Encoders::HEVC_QSV:
      //     return new VideoCodec_HEVC_QSV();
    default:
      return nullptr;
    }
  }
  static auto create(std::string videoCodec) -> BaseVideoCodec * {

    if (codec_map.empty()) {
      initialize();
    }

    auto it = codec_map.find(videoCodec);
    if (it != codec_map.end()) {
      return it->second();
    }

    return nullptr;
  }

private:
  auto static initialize() -> void {

    auto av1 = []() { return new VideoCodec_AV1(); };
    for (auto &alias : VideoCodec_AV1().getAliases()) {
      codec_map[alias] = av1;
    }

    auto av1_amf = []() { return new VideoCodec_AV1_AMF(); };
    for (auto &alias : VideoCodec_AV1_AMF().getAliases()) {
      codec_map[alias] = av1_amf;
    }

    auto av1_nvenc = []() { return new VideoCodec_AV1_NVENC(); };
    for (auto &alias : VideoCodec_AV1_NVENC().getAliases()) {
      codec_map[alias] = av1_nvenc;
    }

    auto h264 = []() { return new VideoCodec_H264(); };
    for (auto &alias : VideoCodec_H264().getAliases()) {
      codec_map[alias] = h264;
    }

    auto h264_amf = []() { return new VideoCodec_H264_AMF(); };
    for (auto &alias : VideoCodec_H264_AMF().getAliases()) {
      codec_map[alias] = h264_amf;
    }

    auto h264_nvenc = []() { return new VideoCodec_H264_NVENC(); };
    for (auto &alias : VideoCodec_H264_NVENC().getAliases()) {
      codec_map[alias] = h264_nvenc;
    }

    auto hevc = []() { return new VideoCodec_HEVC(); };
    for (auto &alias : VideoCodec_HEVC().getAliases()) {
      codec_map[alias] = hevc;
    }

    auto hevc_amf = []() { return new VideoCodec_HEVC_AMF(); };
    for (auto &alias : VideoCodec_HEVC_AMF().getAliases()) {
      codec_map[alias] = hevc_amf;
    }

    auto hevc_nvenc = []() { return new VideoCodec_HEVC_NVENC(); };
    for (auto &alias : VideoCodec_HEVC_NVENC().getAliases()) {
      codec_map[alias] = hevc_nvenc;
    }
  }

  static std::unordered_map<std::string, std::function<BaseVideoCodec *()>>
      codec_map;
};

#endif // VIDEO_CODEC_FACTORY_H
