#include "ProbeResultFormatTags.h"

#include <nlohmann/json.hpp>

#include "src/utils/logging/Logger.h"

ProbeResultFormatTags::ProbeResultFormatTags() {}
ProbeResultFormatTags::~ProbeResultFormatTags() {}

ProbeResultFormatTags::ProbeResultFormatTags(nlohmann::json JSON) {
  LOG_DEBUG(Logger::Priority::INFO, "starting format tags");
  // ProbeResultFormatTags::ENCODER = JSON["ENCODER"];
  (void)JSON;
}