#ifndef VIDEO_CODEC_FACTORY_H
#define VIDEO_CODEC_FACTORY_H

#include "../../../settings/enums/Encoders.h"
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

class VideoCodecFactory {
public:
  static auto createVideoCodec(Encoders videoCodec) -> BaseVideoCodec * {
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
  static auto createVideoCodec(std::string videoCodec) -> BaseVideoCodec * {
    if (videoCodec == "av1") {
      return new VideoCodec_AV1();
    } else if (videoCodec == "av1_amf") {
      return new VideoCodec_AV1_AMF();
    } else if (videoCodec == "av1_nvenc") {
      return new VideoCodec_AV1_NVENC();
    } else if (videoCodec == "h264") {
      return new VideoCodec_H264();
    } else if (videoCodec == "h264_amf") {
      return new VideoCodec_H264_AMF();
    } else if (videoCodec == "h264_nvenc") {
      return new VideoCodec_H264_NVENC();
    } else if (videoCodec == "hevc") {
      return new VideoCodec_HEVC();
    } else if (videoCodec == "hevc_amf") {
      return new VideoCodec_HEVC_AMF();
    } else if (videoCodec == "hevc_nvenc") {
      return new VideoCodec_HEVC_NVENC();
    } else {
      return nullptr;
    }
  };
};

#endif // VIDEO_CODEC_FACTORY_H