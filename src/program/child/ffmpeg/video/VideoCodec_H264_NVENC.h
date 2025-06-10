#ifndef VIDEO_CODEC_H264_NVENC_H
#define VIDEO_CODEC_H264_NVENC_H

#include "BaseVideoCodec.h"

class VideoCodec_H264_NVENC : public BaseVideoCodec {
 public:
  auto getType() -> Encoders override { return Encoders::H264_NVENC; }

  auto controlRateFlag() -> std::string override { return "-crf"; }

  auto supportedLevels() -> std::vector<std::string> override {
    return {
        "1.0",  // Level 1.0 (up to 1280x720, 30fps)
        "1.1",  // Level 1.1 (up to 1280x720, 60fps)
        "1.2",  // Level 1.2 (up to 1280x720, 60fps)
        "1.3",  // Level 1.3 (up to 1280x720, 120fps)
        "2.0",  // Level 2.0 (up to 1920x1080, 30fps)
        "2.1",  // Level 2.1 (up to 1920x1080, 60fps)
        "2.2",  // Level 2.2 (up to 1920x1080, 120fps)
        "3.0",  // Level 3.0 (up to 2048x1080, 30fps)
        "3.1",  // Level 3.1 (up to 2048x1080, 60fps)
        "3.2",  // Level 3.2 (up to 2048x1080, 120fps)
        "4.0",  // Level 4.0 (up to 3840x2160, 30fps)
        "4.1",  // Level 4.1 (up to 3840x2160, 60fps)
        "4.2",  // Level 4.2 (up to 3840x2160, 120fps)
        "5.0",  // Level 5.0 (up to 4096x2160, 30fps)
        "5.1",  // Level 5.1 (up to 4096x2160, 60fps)
        "5.2",  // Level 5.2 (up to 4096x2160, 120fps)
        "6.0",  // Level 6.0 (up to 8192x4320, 30fps)
        "6.1",  // Level 6.1 (up to 8192x4320, 60fps)
        "6.2"   // Level 6.2 (up to 8192x4320, 120fps)
    };
  }

  auto fallbackLevel() -> std::string override { return "5.1"; }

  auto supportedPresets() -> std::vector<std::string> override {
    return {
        "default",       // Balanced preset, providing a good mix of quality and
                         // speed.
        "high_quality",  // Prioritizes better quality at the cost of slower
                         // encoding speed.
        "performance",  // Prioritizes faster encoding speed, resulting in lower
                        // quality and larger file size.
        "low_latency",  // Optimized for low-latency encoding, suitable for
                        // real-time applications.
        "low_latency_high_quality"  // Optimized for low-latency but still
                                    // maintains high quality.
    };
  }

  auto fallbackPreset() -> std::string override { return "high_quality"; }

  auto supportedTunes() -> std::vector<Tunes> override {
    return {Tunes::ZERO_LATENCY,  // Optimized for low latency
            Tunes::GRAIN};
  }

  auto fallbackTune() -> Tunes override { return Tunes::DEFAULT; }
};

#endif  // VIDEO_CODEC_H264_NVENC_H