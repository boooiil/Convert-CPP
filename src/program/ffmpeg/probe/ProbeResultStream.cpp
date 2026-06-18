#include "ProbeResultStream.h"

#include <nlohmann/json.hpp>
#include <string>

#include "ProbeResultStreamDisposition.h"
#include "ProbeResultStreamTags.h"
#include "src/utils/logging/Logger.h"

ProbeResultStream::ProbeResultStream(nlohmann::json JSON)
    : disposition(JSON["disposition"]), tags(JSON["tags"]) {
  LOG_DEBUG(Logger::Priority::INFO, "Starting ProbeResultStream...");
  if (!JSON["index"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "index: ", std::to_string((int)JSON["index"]));
    ProbeResultStream::index = JSON["index"];
  }
  if (!JSON["codec_name"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "codec_name: ", JSON["codec_name"]);
    ProbeResultStream::codec_name = JSON["codec_name"];
  }
  if (!JSON["codec_long_name"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "codec_long_name: ", JSON["codec_long_name"]);
    ProbeResultStream::codec_long_name = JSON["codec_long_name"];
  }
  if (!JSON["codec_tag_string"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "codec_tag_string: ", JSON["codec_tag_string"]);
    ProbeResultStream::codec_tag_string = JSON["codec_tag_string"];
  }
  if (!JSON["codec_tag"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "codec_tag: ", JSON["codec_tag"]);
    ProbeResultStream::codec_tag = JSON["codec_tag"];
  }
  if (!JSON["codec_type"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "codec_type: ", JSON["codec_type"]);
    ProbeResultStream::codec_type = JSON["codec_type"];
  }
  if (!JSON["r_frame_rate"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "r_frame_rate: ", JSON["r_frame_rate"]);
    ProbeResultStream::r_frame_rate = JSON["r_frame_rate"];
  }
  if (!JSON["avg_frame_rate"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "avg_frame_rate: ", JSON["avg_frame_rate"]);
    ProbeResultStream::avg_frame_rate = JSON["avg_frame_rate"];
  }
  if (!JSON["time_base"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "time_base: ", JSON["time_base"]);
    ProbeResultStream::time_base = JSON["time_base"];
  }
  if (!JSON["start_pts"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "start_pts: ", JSON["start_pts"]);
    ProbeResultStream::start_pts = JSON["start_pts"];
  }
  if (!JSON["start_time"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "start_time: ", JSON["start_time"]);
    ProbeResultStream::start_time = JSON["start_time"];
  }
  if (!JSON["extradata_size"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "extradata_size: ", JSON["extradata_size"]);
    ProbeResultStream::extradata_size = JSON["extradata_size"];
  }

  LOG_DEBUG(Logger::Priority::INFO, "Ended ProbeResultStream");
}