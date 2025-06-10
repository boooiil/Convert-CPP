/*********************************************************************
 * @file   MediaProcessConversion.h
 * @brief  Media Process Conversion class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_PROCESS_CONVERSION
#define MEDIA_PROCESS_CONVERSION

#include <string>

#include "Media.h"
#include "MediaProcess.h"

 /**
  * @brief Media process extension for converting media files.
  */
class MediaProcessConversion : public MediaProcess<Media> {
public:
  /**
   * @brief Construct a new Media Process Conversion object
   *
   * @param[out] container - Ptr to Container object.
   * @param[out] media     - Ptr to Media object that will be converted.
   */
  MediaProcessConversion(Media* media);
  ~MediaProcessConversion(void);

  /**
   * @brief Parse the data from the process.
   *
   * @param[in] data -  Data parsed from the running FFmpeg process.
   */
  void parse(std::string data) override;
};

#endif  // !MEDIA_PROCESS_CONVERSION
