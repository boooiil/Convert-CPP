#include "MediaVideoProperties.h"

#include "../../../utils/logging/Logger.h"

MediaVideoProperties::MediaVideoProperties()
    : fps(0.0),
      totalFrames(0),
      width(0),
      height(0),
      crf(0),
      convertedResolution(""),
      subtitleProvider(""),
      convertedHeight(""),
      convertedWidth(""),
      ratio(""),
      crop("") {}

MediaVideoProperties::~MediaVideoProperties() {
  LOG_DEBUG("Deconstructing MediaVideoProperties");
}