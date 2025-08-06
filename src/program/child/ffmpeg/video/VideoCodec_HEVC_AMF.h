#ifndef VIDEO_CODEC_HEVC_AMF_H
#define VIDEO_CODEC_HEVC_AMF_H

#include "BaseVideoCodec.h"

class VideoCodec_HEVC_AMF : public BaseVideoCodec {
 public:
  VideoCodec_HEVC_AMF(std::string runningPreset = "high_quality",
                      std::string runningLevel = "4.1",
                      Tunes runningTune = Tunes::DEFAULT)
      : BaseVideoCodec(runningPreset, runningLevel, runningTune) {};

  auto getType() -> Encoders override { return Encoders::HEVC_AMF; }
  auto getName() -> std::string override { return "hevc_amf"; }

  auto controlRateFlag() -> std::string override {
    return "-crf";
  }  // AMD uses -rc cqp -qp_i %% -qp_p %% -qp_b %%

  auto supportedLevels() -> std::vector<std::string> override {
    return {
        "1.0",  // Level 1.0: Supports resolutions up to 1280x720 at low
                // bitrates.
        "1.1",  // Level 1.1: Supports resolutions up to 1280x720 with slightly
                // higher bitrate than 1.0.
        "2.0",  // Level 2.0: Supports up to 1080p resolution with higher
                // bitrate.
        "2.1",  // Level 2.1: Supports up to 1080p resolution with higher frame
                // rate and bitrate.
        "3.0",  // Level 3.0: Supports up to 2160p (4K) resolution with high
                // bitrate.
        "3.1",  // Level 3.1: Supports 4K resolution with even higher bitrate.
        "4.0",  // Level 4.0: Supports up to 4K resolution with advanced bitrate
                // and performance.
        "4.1",  // Level 4.1: Supports 4K resolution with high bitrate and
                // demanding performance.
        "5.0",  // Level 5.0: Supports 4K or higher resolutions with very high
                // bitrate and advanced encoding.
        "5.1",  // Level 5.1: Supports higher resolutions and bitrates for
                // professional use.
        "5.2"   // Level 5.2: High-end professional encoding, supports high
                // bitrate and very high resolutions (up to 8K).
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
    return {Tunes::FILM,
            Tunes::ZERO_LATENCY,  // Optimized for low latency
            Tunes::GRAIN};
  }

  auto fallbackTune() -> Tunes override { return Tunes::DEFAULT; }
};

#endif  // VIDEO_CODEC_HEVC_AMF_H