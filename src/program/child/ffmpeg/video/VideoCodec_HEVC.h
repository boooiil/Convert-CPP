#ifndef VIDEO_CODEC_HEVC_H
#define VIDEO_CODEC_HEVC_H

#include "BaseVideoCodec.h"

class VideoCodec_HEVC : public BaseVideoCodec {
 public:
  VideoCodec_HEVC(std::string runningPreset = "slow",
                  std::string runningLevel = "4.1",
                  Tunes runningTune = Tunes::DEFAULT)
      : BaseVideoCodec(runningPreset, runningLevel, runningTune) {};

  auto getType() -> Encoders override { return Encoders::HEVC; }
  auto getName() -> std::string override { return "hevc"; }

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
        "ultrafast",  // Fastest encoding speed, but least efficient (largest
                      // file size).
        "superfast",  // Very fast encoding with relatively high file size.
        "veryfast",   // Fast encoding with a moderate file size.
        "faster",     // Balanced encoding speed with reasonable file size.
        "fast",       // Slightly slower but more efficient encoding.
        "medium",     // Default preset, a good balance between speed and
                      // compression.
        "slow",       // Slower encoding with better compression efficiency.
        "slower",     // Even slower encoding, with improved compression.
        "veryslow",   // Slowest encoding with the best compression efficiency.
        "placebo"     // Extremely slow encoding, but with the best possible
                      // compression.
    };
  }

  auto fallbackPreset() -> std::string override { return "slow"; }

  auto supportedTunes() -> std::vector<Tunes> override {
    return {Tunes::PSNR,  // Optimizes for Peak Signal-to-Noise Ratio (PSNR)
            Tunes::SSIM,  // Optimizes for Structural Similarity Index (SSIM)
            Tunes::ZERO_LATENCY,  // Optimized for low latency
            Tunes::GRAIN};
  }

  auto fallbackTune() -> Tunes override { return Tunes::DEFAULT; }
};

#endif  // VIDEO_CODEC_HEVC_H