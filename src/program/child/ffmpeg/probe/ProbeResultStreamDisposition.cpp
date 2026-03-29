#include "ProbeResultStreamDisposition.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"

/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */
ProbeResultStreamDisposition::ProbeResultStreamDisposition(
    nlohmann::json JSON) {
  LOG_DEBUG("Starting ProbeResultStreamDisposition...");
  ProbeResultStreamDisposition::default_ = JSON["default"];
  ProbeResultStreamDisposition::dub = JSON["dub"];
  ProbeResultStreamDisposition::original = JSON["original"];
  ProbeResultStreamDisposition::comment = JSON["comment"];
  ProbeResultStreamDisposition::lyrics = JSON["lyrics"];
  ProbeResultStreamDisposition::karaoke = JSON["karaoke"];
  ProbeResultStreamDisposition::forced = JSON["forced"];
  ProbeResultStreamDisposition::hearing_impaired = JSON["hearing_impaired"];
  ProbeResultStreamDisposition::visual_impaired = JSON["visual_impaired"];
  ProbeResultStreamDisposition::clean_effects = JSON["clean_effects"];
  ProbeResultStreamDisposition::attached_pic = JSON["attached_pic"];
  ProbeResultStreamDisposition::timed_thumbnails = JSON["timed_thumbnails"];
  /* Does not appear on linux (ubuntu), apt, ffmpeg=4.4.2-0ubuntu0.22.04.1 */
  // ProbeResultStreamDisposition::captions = JSON["captions"];
  // ProbeResultStreamDisposition::descriptions = JSON["descriptions"];
  // ProbeResultStreamDisposition::metadata = JSON["metadata"];
  // ProbeResultStreamDisposition::dependent = JSON["dependent"];
  // ProbeResultStreamDisposition::still_image = JSON["still_image"];
  LOG_DEBUG("Ended ProbeResultStreamDisposition");
}