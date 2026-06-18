#include "ProbeResultStreamVideo.h"

#include <nlohmann/json.hpp>
#include <regex>

#include "ProbeResultStream.h"
#include "src/utils/RegexUtils.h"
#include "src/utils/logging/Logger.h"

/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */
ProbeResultStreamVideo::ProbeResultStreamVideo(nlohmann::json JSON)
    : ProbeResultStream(JSON) {
  LOG_DEBUG(Logger::Priority::INFO, "Starting ProbeResultStreamVideo...");

  if (!JSON["is_avc"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "is_avc: ", JSON["is_avc"]);
    ProbeResultStreamVideo::is_avc = RegexUtils::isMatch(
        JSON["is_avc"], "true", std::regex_constants::icase);
  }
  if (!JSON["width"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "width: ", std::to_string((int)JSON["width"]));
    ProbeResultStreamVideo::width = JSON["width"];
  }
  if (!JSON["height"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "height: ", std::to_string((int)JSON["height"]));
    ProbeResultStreamVideo::height = JSON["height"];
  }
  if (!JSON["coded_width"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "coded_width: ", std::to_string((int)JSON["coded_width"]));
    ProbeResultStreamVideo::coded_width = JSON["coded_width"];
  }
  if (!JSON["coded_height"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "coded_height: ", std::to_string((int)JSON["coded_height"]));
    ProbeResultStreamVideo::coded_height = JSON["coded_height"];
  }
  if (!JSON["closed_captions"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "closed_captions: ",
                std::to_string((int)JSON["closed_captions"]));
    ProbeResultStreamVideo::closed_captions = JSON["closed_captions"];
  }
  if (!JSON["has_b_frames"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "has_b_frames: ", std::to_string((int)JSON["has_b_frames"]));
    ProbeResultStreamVideo::has_b_frames = JSON["has_b_frames"];
  }
  if (!JSON["sample_aspect_ratio"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "sample_aspect_ratio: ", JSON["sample_aspect_ratio"]);
    ProbeResultStreamVideo::sample_aspect_ratio = JSON["sample_aspect_ratio"];
  }
  if (!JSON["display_aspect_ratio"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "display_aspect_ratio: ", JSON["display_aspect_ratio"]);
    ProbeResultStreamVideo::display_aspect_ratio = JSON["display_aspect_ratio"];
  }
  if (!JSON["pix_fmt"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "pix_fmt: ", JSON["pix_fmt"]);
    ProbeResultStreamVideo::pix_fmt = JSON["pix_fmt"];
  }
  if (!JSON["level"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "level: ", std::to_string((int)JSON["level"]));
    ProbeResultStreamVideo::level = JSON["level"];
  }
  if (!JSON["color_range"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "color_range: ", JSON["color_range"]);
    ProbeResultStreamVideo::color_range = JSON["color_range"];
  }
  if (!JSON["color_space"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "color_space: ", JSON["color_space"]);
    ProbeResultStreamVideo::color_space = JSON["color_space"];
  }
  if (!JSON["color_transfer"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "color_transfer: ", JSON["color_transfer"]);
    ProbeResultStreamVideo::color_transfer = JSON["color_transfer"];
  }
  if (!JSON["color_primaries"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "color_primaries: ", JSON["color_primaries"]);
    ProbeResultStreamVideo::color_primaries = JSON["color_primaries"];
  }
  if (!JSON["chroma_location"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "chroma_location: ", JSON["chroma_location"]);
    ProbeResultStreamVideo::chroma_location = JSON["chroma_location"];
  }
  if (!JSON["field_order"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "field_order: ", JSON["field_order"]);
    ProbeResultStreamVideo::field_order = JSON["field_order"];
  }
  if (!JSON["refs"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "refs: ", std::to_string((int)JSON["refs"]));
    ProbeResultStreamVideo::refs = JSON["refs"];
  }
  if (!JSON["nal_length_size"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "nal_length_size: ", JSON["nal_length_size"]);
    ProbeResultStreamVideo::nal_length_size = JSON["nal_length_size"];
  }
  if (!JSON["bits_per_raw_sample"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "bits_per_raw_sample: ", JSON["bits_per_raw_sample"]);
    ProbeResultStreamVideo::bits_per_raw_sample = JSON["bits_per_raw_sample"];
  }

  LOG_DEBUG(Logger::Priority::INFO, "Ended ProbeResultStreamVideo");
}