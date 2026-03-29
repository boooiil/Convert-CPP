#include "ProbeResultStreamVideo.h"

#include <nlohmann/json.hpp>
#include <regex>

#include "../../../../utils/RegexUtils.h"
#include "../../../../utils/logging/Logger.h"
#include "ProbeResultStream.h"

/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */
ProbeResultStreamVideo::ProbeResultStreamVideo(nlohmann::json JSON)
    : ProbeResultStream(JSON) {
  LOG_DEBUG("Starting ProbeResultStreamVideo...");

  if (!JSON["is_avc"].is_null()) {
    LOG_VERBOSE("is_avc: ", JSON["is_avc"]);
    ProbeResultStreamVideo::is_avc = RegexUtils::isMatch(
        JSON["is_avc"], "true", std::regex_constants::icase);
  }
  /* Does not appear on linux (ubuntu), apt, ffmpeg=4.4.2-0ubuntu0.22.04.1 */
  // ProbeResultStreamVideo::extradata_size = JSON["extradata_size"];
  LOG_VERBOSE("width: ", std::to_string((int)JSON["width"]));
  ProbeResultStreamVideo::width = JSON["width"];
  LOG_VERBOSE("height: ", std::to_string((int)JSON["height"]));
  ProbeResultStreamVideo::height = JSON["height"];
  LOG_VERBOSE("coded_width: ", std::to_string((int)JSON["coded_width"]));
  ProbeResultStreamVideo::coded_width = JSON["coded_width"];
  LOG_VERBOSE("coded_height: ", std::to_string((int)JSON["coded_height"]));
  ProbeResultStreamVideo::coded_height = JSON["coded_height"];
  LOG_VERBOSE("closed_captions: ",
              std::to_string((int)JSON["closed_captions"]));
  ProbeResultStreamVideo::closed_captions = JSON["closed_captions"];
  /* Does not appear on linux (ubuntu), apt, ffmpeg=4.4.2-0ubuntu0.22.04.1 */
  // ProbeResultStreamVideo::film_grain = JSON["film_grain"];
  LOG_VERBOSE("has_b_frames: ", std::to_string((int)JSON["has_b_frames"]));
  ProbeResultStreamVideo::has_b_frames = JSON["has_b_frames"];
  if (!JSON["sample_aspect_ratio"].is_null()) {
    LOG_VERBOSE("sample_aspect_ratio: ", JSON["sample_aspect_ratio"]);
    ProbeResultStreamVideo::sample_aspect_ratio = JSON["sample_aspect_ratio"];
  }
  if (!JSON["display_aspect_ratio"].is_null()) {
    LOG_VERBOSE("display_aspect_ratio: ", JSON["display_aspect_ratio"]);
    ProbeResultStreamVideo::display_aspect_ratio = JSON["display_aspect_ratio"];
  }
  if (!JSON["pix_fmt"].is_null()) {
    LOG_VERBOSE("pix_fmt: ", JSON["pix_fmt"]);
    ProbeResultStreamVideo::pix_fmt = JSON["pix_fmt"];
  }
  if (!JSON["level"].is_null()) {
    LOG_VERBOSE("level: ", std::to_string((int)JSON["level"]));
    ProbeResultStreamVideo::level = JSON["level"];
  }
  if (!JSON["color_range"].is_null()) {
    LOG_VERBOSE("color_range: ", JSON["color_range"]);
    ProbeResultStreamVideo::color_range = JSON["color_range"];
  }
  if (!JSON["color_space"].is_null()) {
    LOG_VERBOSE("color_space: ", JSON["color_space"]);
    ProbeResultStreamVideo::color_space = JSON["color_space"];
  }
  if (!JSON["color_transfer"].is_null()) {
    LOG_VERBOSE("color_transfer: ", JSON["color_transfer"]);
    ProbeResultStreamVideo::color_transfer = JSON["color_transfer"];
  }
  if (!JSON["color_primaries"].is_null()) {
    LOG_VERBOSE("color_primaries: ", JSON["color_primaries"]);
    ProbeResultStreamVideo::color_primaries = JSON["color_primaries"];
  }
  if (!JSON["chroma_location"].is_null()) {
    LOG_VERBOSE("chroma_location: ", JSON["chroma_location"]);
    ProbeResultStreamVideo::chroma_location = JSON["chroma_location"];
  }
  if (!JSON["field_order"].is_null()) {
    LOG_VERBOSE("field_order: ", JSON["field_order"]);
    ProbeResultStreamVideo::field_order = JSON["field_order"];
  }
  if (!JSON["refs"].is_null()) {
    LOG_VERBOSE("refs: ", std::to_string((int)JSON["refs"]));
    ProbeResultStreamVideo::refs = JSON["refs"];
  }
  if (!JSON["nal_length_size"].is_null()) {
    LOG_VERBOSE("nal_length_size: ", JSON["nal_length_size"]);
    ProbeResultStreamVideo::nal_length_size = JSON["nal_length_size"];
  }
  if (!JSON["bits_per_raw_sample"].is_null()) {
    LOG_VERBOSE("bits_per_raw_sample: ", JSON["bits_per_raw_sample"]);
    ProbeResultStreamVideo::bits_per_raw_sample = JSON["bits_per_raw_sample"];
  }

  LOG_DEBUG("Ended ProbeResultStreamVideo");
}