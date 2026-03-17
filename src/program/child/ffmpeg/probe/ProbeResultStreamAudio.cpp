#include "ProbeResultStreamAudio.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"
#include "ProbeResultStreamDisposition.h"
#include "ProbeResultStreamTags.h"

/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */

ProbeResultStreamAudio::ProbeResultStreamAudio()
    : channels(-1), bits_per_sample(-1), initial_padding(-1) {}
ProbeResultStreamAudio::~ProbeResultStreamAudio() {}

ProbeResultStreamAudio::ProbeResultStreamAudio(nlohmann::json JSON)
    : channels(-1), bits_per_sample(-1), initial_padding(-1) {
  LOG_DEBUG("Starting ProbeResultStreamAudio...");

  if (!JSON["index"].is_null()) {
    LOG_VERBOSE("index: ", std::to_string((int)JSON["index"]));
    ProbeResultStreamAudio::index = JSON["index"];
  }
  if (!JSON["codec_name"].is_null()) {
    LOG_VERBOSE("codec_name: ", JSON["codec_name"]);
    ProbeResultStreamAudio::codec_name = JSON["codec_name"];
  }
  if (!JSON["codec_long_name"].is_null()) {
    LOG_VERBOSE("codec_long_name: ", JSON["codec_long_name"]);
    ProbeResultStreamAudio::codec_long_name = JSON["codec_long_name"];
  }
  if (!JSON["codec_tag_string"].is_null()) {
    LOG_VERBOSE("codec_tag_string: ", JSON["codec_tag_string"]);
    ProbeResultStreamAudio::codec_tag_string = JSON["codec_tag_string"];
  }
  if (!JSON["codec_tag"].is_null()) {
    LOG_VERBOSE("codec_tag: ", JSON["codec_tag"]);
    ProbeResultStreamAudio::codec_tag = JSON["codec_tag"];
  }
  if (!JSON["codec_type"].is_null()) {
    LOG_VERBOSE("codec_type: ", JSON["codec_type"]);
    ProbeResultStreamAudio::codec_type = JSON["codec_type"];
  }
  if (!JSON["r_frame_rate"].is_null()) {
    LOG_VERBOSE("r_frame_rate: ", JSON["r_frame_rate"]);
    ProbeResultStreamAudio::r_frame_rate = JSON["r_frame_rate"];
  }
  if (!JSON["avg_frame_rate"].is_null()) {
    LOG_VERBOSE("avg_frame_rate: ", JSON["avg_frame_rate"]);
    ProbeResultStreamAudio::avg_frame_rate = JSON["avg_frame_rate"];
  }
  if (!JSON["time_base"].is_null()) {
    LOG_VERBOSE("time_base: ", JSON["time_base"]);
    ProbeResultStreamAudio::time_base = JSON["time_base"];
  }
  if (!JSON["start_pts"].is_null()) {
    LOG_VERBOSE("start_pts: ", std::to_string((int)JSON["start_pts"]));
    ProbeResultStreamAudio::start_pts = JSON["start_pts"];
  }
  if (!JSON["start_time"].is_null()) {
    LOG_VERBOSE("start_time: ", JSON["start_time"]);
    ProbeResultStreamAudio::start_time = JSON["start_time"];
  }
  if (!JSON["extradata_size"].is_null()) {
    LOG_VERBOSE("extradata_size: ",
                std::to_string((int)JSON["extradata_size"]));
    ProbeResultStreamAudio::extradata_size = JSON["extradata_size"];
  }

  ProbeResultStreamAudio::disposition =
      ProbeResultStreamDisposition(JSON["disposition"]);

  ProbeResultStreamAudio::tags = ProbeResultStreamTags(JSON["tags"]);

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