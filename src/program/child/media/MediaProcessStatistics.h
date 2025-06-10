/*********************************************************************
 * @file   MediaProcessStatistics.h
 * @brief  Media Process Statistics class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_PROCESS_STATISTICS
#define MEDIA_PROCESS_STATISTICS

#include "Media.h"
#include "MediaProcess.h"
#include <string>

 /**
  * @brief Media process extension for obtaining statistics for the current media
  * file.
  */
class MediaProcessStatistics : public MediaProcess<Media> {
public:
  /**
   * @brief Construct a new Media Process Statistics object.
   *
   * @param[out] container - Container object.
   * @param[out] media     - Media object that will be getting statistics.
   */
  MediaProcessStatistics(Media* media);
  ~MediaProcessStatistics(void);

  /**
   * @brief Start the process.
   *
   * @param[in] command - Command to be executed.
   */
  void start(std::string command) override;

  /**
   * @brief Parse the data from the process.
   *
   * @param[in] data -  Data parsed from the running FFmpeg process.
   */
  void parse(std::string data) override;
};

#endif  // !MEDIA_PROCESS_STATISTICS
