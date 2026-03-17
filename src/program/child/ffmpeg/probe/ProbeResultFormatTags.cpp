#include "ProbeResultFormatTags.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"

ProbeResultFormatTags::ProbeResultFormatTags() {}
ProbeResultFormatTags::~ProbeResultFormatTags() {}

ProbeResultFormatTags::ProbeResultFormatTags(nlohmann::json JSON) {
  LOG_DEBUG("starting format tags");
  // ProbeResultFormatTags::ENCODER = JSON["ENCODER"];
  (void)JSON;
}