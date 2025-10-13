#ifndef VIDEO_CODEC_AV1_NVENC_H
#define VIDEO_CODEC_AV1_NVENC_H

#include "BaseVideoCodec.h"

class VideoCodec_AV1_NVENC : public BaseVideoCodec {
public:
  VideoCodec_AV1_NVENC(std::string runningPreset = "high_quality",
                       std::string runningLevel = "4.1",
                       Tunes runningTune = Tunes::DEFAULT)
      : BaseVideoCodec(runningPreset, runningLevel, runningTune) {};

  auto getType() -> Encoders override { return Encoders::AV1_NVENC; }
  auto getName() -> std::string override { return "av1_nvenc"; }
  auto getAliases() -> std::set<std::string> override {
    return {"av1_nvenc", "nvenc_av1", "nvidia_av1", "av1_nvidia"};
  }

  auto controlRateFlag() -> std::string override { return "-crf"; }

  auto supportedLevels() -> std::vector<std::string> override {
    return {
        "1.0", // Level 1.0: Supports up to 720p resolution and lower bitrate.
        "1.1", // Level 1.1: Supports up to 1080p resolution with a bit higher
               // bitrate than 1.0.
        "2.0", // Level 2.0: Supports up to 1080p resolution with increased
               // bitrate and frame rate.
        "2.1", // Level 2.1: Supports up to 1440p (2K) resolution with higher
               // bitrate.
        "3.0", // Level 3.0: Supports up to 2160p (4K) resolution with high
               // bitrate.
        "3.1", // Level 3.1: Supports up to 4K resolution with even higher
               // bitrate and frame rate.
        "4.0", // Level 4.0: Supports up to 4K with higher bitrate and more
               // demanding performance.
        "4.1", // Level 4.1: Supports higher 4K resolution, frame rate, and
               // bitrate.
        "5.0", // Level 5.0: Supports very high resolutions and bitrates for
               // professional-grade video.
        "5.1", // Level 5.1: High-quality encoding with more advanced
               // performance and bitrate handling.
        "5.2"  // Level 5.2: High-end professional encoding for very
               // high-resolution and high-bitrate video.
    };
  }

  auto fallbackLevel() -> std::string override { return "5.1"; }

  auto supportedPresets() -> std::vector<std::string> override {
    return {
        "default",      // Balanced preset, providing a good trade-off between
                        // quality and speed.
        "high_quality", // Prioritizes better quality at the cost of slower
                        // encoding speed.
        "performance",  // Prioritizes faster encoding speed, with lower quality
                        // and larger file sizes.
        "low_latency",  // Optimized for low-latency encoding, suitable for
                        // real-time applications.
        "lossless", // Maximizes quality with lossless encoding, resulting in
                    // very large file sizes.
        "high_performance" // Optimized for the fastest possible encoding
                           // speed, sacrificing quality for speed.
    };
  }

  auto fallbackPreset() -> std::string override { return "high_quality"; }

  auto supportedTunes() -> std::vector<Tunes> override {
    return {Tunes::ZERO_LATENCY, // Optimized for low latency
            Tunes::GRAIN};
  }

  auto fallbackTune() -> Tunes override { return Tunes::DEFAULT; }
};

#endif // VIDEO_CODEC_AV1_NVENC_H