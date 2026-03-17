#ifndef MEDIA_FORMAT_H
#define MEDIA_FORMAT_H

#include <vector>

#include "../../child/media/MediaFormat.h"
#include "ObjectEnumDataHolder.h"

class MediaFormats {
 public:
  static const ObjectEnumDataHolder<MediaFormat> CUSTOM;

  static const ObjectEnumDataHolder<MediaFormat> P_480;
  static const ObjectEnumDataHolder<MediaFormat> PC_480;

  static const ObjectEnumDataHolder<MediaFormat> P_720;
  static const ObjectEnumDataHolder<MediaFormat> PM_720;
  static const ObjectEnumDataHolder<MediaFormat> PN_720;

  static const ObjectEnumDataHolder<MediaFormat> P_1080;
  static const ObjectEnumDataHolder<MediaFormat> PM_1080;
  static const ObjectEnumDataHolder<MediaFormat> PN_1080;

  static const ObjectEnumDataHolder<MediaFormat> P_1440;
  static const ObjectEnumDataHolder<MediaFormat> P_2160;

  static const std::vector<const ObjectEnumDataHolder<MediaFormat>*> _all() {
    return {&CUSTOM, &P_480,   &PC_480,  &P_720,  &PM_720, &PN_720,
            &P_1080, &PM_1080, &PN_1080, &P_1440, &P_2160};
  }

  static const ObjectEnumDataHolder<MediaFormat> getKey(MediaFormat value) {
    for (const auto* data : _all()) {
      if (data->getName() == value.name) {
        return *data;
      }
    }
    return CUSTOM;
  }
};

#endif  // MEDIA_DEFINED_FORMAT_H