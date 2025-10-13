#ifndef VIDEO_CODEC_AV1_H
#define VIDEO_CODEC_AV1_H

#include "BaseVideoCodec.h"

class VideoCodec_AV1 : public BaseVideoCodec {
public:
  VideoCodec_AV1(std::string runningPreset = "2",
                 std::string runningLevel = "4.0",
                 Tunes runningTune = Tunes::DEFAULT)
      : BaseVideoCodec(runningPreset, runningLevel, runningTune) {};

  auto getType() -> Encoders override { return Encoders::AV1; }
  auto getName() -> std::string override { return "av1"; }
  auto getAliases() -> std::set<std::string> override {
    return {"av1", "av1_main", "av1_high", "av1_profile"};
  }

  auto controlRateFlag() -> std::string override { return "-crf"; }

  auto supportedLevels() -> std::vector<std::string> override {
    return {
        "1.0", // Level 1.0 (up to 1920x1080, 30fps)
        "2.0", // Level 2.0 (up to 1920x1080, 60fps)
        "3.0", // Level 3.0 (up to 3840x2160, 30fps)
        "4.0", // Level 4.0 (up to 3840x2160, 60fps)
        "5.0", // Level 5.0 (up to 7680x4320, 30fps)
        "6.0", // Level 6.0 (up to 7680x4320, 60fps)
        "7.0"  // Level 7.0 (up to 7680x4320, 120fps)
    };
  }

  auto fallbackLevel() -> std::string override { return "5.1"; }

  auto supportedPresets() -> std::vector<std::string> override {
    return {
        "0", // Best (Placebo)
        "1", // Better (Very Slow)
        "2", // Good (Slow)
        "3", // Faster
        "4", // Fast
        "5", // Very Fast
        "6"  // Super Fast
    };
  }

  auto fallbackPreset() -> std::string override { return "2"; }

  auto supportedTunes() -> std::vector<Tunes> override {
    return {
        Tunes::PSNR, // Optimizes for Peak Signal-to-Noise Ratio (PSNR)
        Tunes::SSIM, // Optimizes for Structural Similarity Index (SSIM)
    };
  }

  auto fallbackTune() -> Tunes override { return Tunes::DEFAULT; }
};

#endif // VIDEO_CODEC_AV1_H