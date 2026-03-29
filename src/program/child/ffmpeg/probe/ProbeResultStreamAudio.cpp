#include "ProbeResultStreamAudio.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"
/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */

ProbeResultStreamAudio::ProbeResultStreamAudio(nlohmann::json JSON)
    : ProbeResultStream(JSON), channels(-1), bits_per_sample(-1),
      initial_padding(-1) {
  LOG_DEBUG("Starting ProbeResultStreamAudio...");

  if (!JSON["sample_fmt"].is_null()) {
    LOG_VERBOSE("sample_fmt: ", JSON["sample_fmt"]);
    ProbeResultStreamAudio::sample_fmt = JSON["sample_fmt"];
  }
  if (!JSON["sample_rate"].is_null()) {
    LOG_VERBOSE("sample_rate: ", JSON["sample_rate"]);
    std::string result = JSON["sample_rate"];
    ProbeResultStreamAudio::sample_rate = std::stoi(result);
  }
  if (!JSON["channels"].is_null()) {
    LOG_VERBOSE("channels: ", std::to_string((int)JSON["channels"]));
    ProbeResultStreamAudio::channels = JSON["channels"];
  }
  if (!JSON["channel_layout"].is_null()) {
    LOG_VERBOSE("channel_layout: ", JSON["channel_layout"]);
    ProbeResultStreamAudio::channel_layout = JSON["channel_layout"];
  }
  if (!JSON["bits_per_sample"].is_null()) {
    LOG_VERBOSE("bits_per_sample: ",
                std::to_string((int)JSON["bits_per_sample"]));
    ProbeResultStreamAudio::bits_per_sample = JSON["bits_per_sample"];
  }
  if (!JSON["initial_padding"].is_null()) {
    LOG_VERBOSE("initial_padding: ",
                std::to_string((int)JSON["initial_padding"]));
    ProbeResultStreamAudio::initial_padding = JSON["initial_padding"];
  }
  LOG_DEBUG("Ended ProbeResultStreamAudio");
}