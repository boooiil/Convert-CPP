/*********************************************************************
 * @file   MediaDefinedFormat.h
 * @brief  MediaDefinedFormat class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_DEFINED_FORMAT
#define MEDIA_DEFINED_FORMAT

#include <string>
#include <unordered_map>

#include "MediaFormat.h"

/**
 * @brief Holds defined media formats.
 */
class MediaDefinedFormat {
 public:
  /// @brief Map of media formats {name, MediaFormat}
  static std::unordered_map<std::string, MediaFormat> formats;
};

#endif  // !MEDIA_DEFINED_FORMAT
