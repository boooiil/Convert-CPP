/*********************************************************************
 * @file   MediaProcessValidate.h
 * @brief  Media Process Validate header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_PROCESS_VALIDATE
#define MEDIA_PROCESS_VALIDATE

#include <string>

#include "Media.h"
#include "MediaProcess.h"

 /**
  * @brief Media process extension for validating media files.
  */
class MediaProcessValidate : public MediaProcess<Media> {
public:
  /**
   * @brief Construct a new Media Process Validate object.
   *
   * @param[out] container - Container object.
   * @param[out] media     - Media object that will be validating.
   */
  MediaProcessValidate(Media* media);
  ~MediaProcessValidate(void);

  /**
   * @brief Parse data for updating MediaWorkingProperties.
   *
   * @param[in] data - Data parsed from the running FFmpeg process.
   */
  void parse(std::string data) override;
};

#endif  // !MEDIA_PROCESS_VALIDATE
