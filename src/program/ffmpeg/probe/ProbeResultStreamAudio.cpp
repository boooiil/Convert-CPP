#include "ProbeResultStreamAudio.h"

#include <nlohmann/json.hpp>

#include "src/utils/logging/Logger.h"
/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */

ProbeResultStreamAudio::ProbeResultStreamAudio(nlohmann::json JSON)
    : ProbeResultStream(JSON), channels(-1), bits_per_sample(-1),
      initial_padding(-1) {
  LOG_DEBUG(Logger::Priority::INFO, "Starting ProbeResultStreamAudio...");

  if (!JSON["sample_fmt"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "sample_fmt: ", JSON["sample_fmt"]);
    ProbeResultStreamAudio::sample_fmt = JSON["sample_fmt"];
  }
  if (!JSON["sample_rate"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "sample_rate: ", JSON["sample_rate"]);
    std::string result = JSON["sample_rate"];
    ProbeResultStreamAudio::sample_rate = std::stoi(result);
  }
  if (!JSON["channels"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "channels: ", std::to_string((int)JSON["channels"]));
    ProbeResultStreamAudio::channels = JSON["channels"];
  }
  if (!JSON["channel_layout"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "channel_layout: ", JSON["channel_layout"]);
    ProbeResultStreamAudio::channel_layout = JSON["channel_layout"];
  }
  if (!JSON["bits_per_sample"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "bits_per_sample: ",
                std::to_string((int)JSON["bits_per_sample"]));
    ProbeResultStreamAudio::bits_per_sample = JSON["bits_per_sample"];
  }
  if (!JSON["initial_padding"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "initial_padding: ",
                std::to_string((int)JSON["initial_padding"]));
    ProbeResultStreamAudio::initial_padding = JSON["initial_padding"];
  }
  LOG_DEBUG(Logger::Priority::INFO, "Ended ProbeResultStreamAudio");
}