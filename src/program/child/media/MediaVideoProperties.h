/*********************************************************************
 * @file   MediaVideoProperties.h
 * @brief  Media video properties header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_VIDEO_PROPERTIES
#define MEDIA_VIDEO_PROPERTIES

#include <string>

/**
 * @brief Holds information about the media video properties.
 */
class MediaVideoProperties {
public:
  /**
   * @brief Construct a new Media Video Properties object.
   *
   */
  MediaVideoProperties(void);
  ~MediaVideoProperties(void);

  float fps; /// @brief FPS of the media.

  unsigned long long totalFrames; /// @brief Total frames of the media.
  int width;                      /// @brief Width of the media.
  int height;                     /// @brief Height of the media.
  int crf;                        /// @brief CRF of the media.

  /// @brief The desired resolution of the media for conversion.
  std::string convertedResolution;
  /// @brief The desired subtitle provider of the media for conversion.
  std::string subtitleProvider;
  /// @brief The desired height of the media for conversion.
  std::string convertedHeight;
  /// @brief The desired width of the media for conversion.
  std::string convertedWidth;
  /// @brief The desired ratio of the media for conversion.
  std::string ratio;
  /// @brief The desired crop of the media for conversion.
  std::string crop;
};

#endif // !MEDIA_VIDEO_PROPERTIES
