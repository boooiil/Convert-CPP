/*********************************************************************
 * @file   Media.h
 * @brief  Media class header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA
#define MEDIA

#include <uuid.h>

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "../../generics/JSONSerializable.h"
#include "../../settings/enums/Activity_N.h"
#include "../ffmpeg/FFmpegArgumentBuilder.h"
#include "../ffmpeg/probe/ProbeResult.h"
#include "MediaFile.h"
#include "src/program/child/media/file/FileContainer.h"
#include "src/program/settings/options/ChildOptions.h"

/**
 * @brief Media class.
 *
 * Trrr
 *
 */
class Media : public JSONSerializable {
public:
  uuids::uuid id; /// @brief unique identifier
  long started;   /// @brief start time
  long ended;     /// @brief end time

  ProbeResult *probeResult; /// @brief ProbeResult object
  FFmpegArgumentBuilder
      *ffmpegArguments; /// @brief vector of arguments to be passed to ffmpeg

  /**
   * @brief Create a Media object. (unused)
   *
   */
  Media();

  /**
   * @brief Create a Media object.
   *
   * @param[in] name - Name of the file.
   * @param[in] path - Path to the file (generally CWD).
   */
  Media(ChildOptions &options, uuids::uuid id, std::string name,
        std::string path);
  ~Media(void);

  /**
   * @brief Set the activity type.
   *
   * @param[in] activity - Activity type.
   */
  void setActivity(Activity_N::Activity activity);

  /**
   * @brief Do the statistics process.
   *
   * @param[out] container - Ptr to the Container object.
   */
  void doStatistics();

  /**
   * @brief Do the conversion process.
   *
   * @param[out] container - Ptr to the Container object.
   */
  void doConversion();

  /**
   * @brief Do the validation process.
   *
   * @param[out] container - Ptr to the Container object.
   */
  void doValidation();

  /**
   * @brief Build arguments required to process FFmpeg media with given
   * parameters.
   *
   * @param[in] isValidate  - TODO: I'm not sure what this does.
   */
  void buildFFmpegArguments(bool isValidate);

  /**
   * @brief Get the activity type.
   *
   * @return Activity_N::Activity - Activity type.
   */
  Activity_N::Activity getActivity(void);

  /**
   * @brief Check if the current media is processing.
   *
   * @return true if the media activity is STATISTICS, CONVERT or VALIDATE.
   * @return false if the media activity is WAITING-like, FINISHED, or
   * FAILED-like.
   */
  const bool isProcessing(void) const;

  /**
   * @brief Check if the current media has failed processing.
   *
   * @return true if the media activity is FAILED-like.
   */
  const bool hasFailed(void) const;

  /**
   * @brief Check if the current media has failed processing.
   *
   * @return true if the media activity is FINISHED.
   */
  const bool hasFinished(void) const;

  /**
   * @brief Check if the current media is waiting to be processed.
   *
   * @return true if the media activity is WAITING.
   */
  const bool isWaiting(void) const;

  /**
   * @brief Check if the current media is waiting for statistics.
   *
   * @return true if the media activity is WAITING_STATISTICS.
   */
  const bool isWaitingToStatistics(void) const;

  /**
   * @brief Check if the current media is waiting to convert.
   *
   * @return true if the media activity is WAITING_CONVERT.
   */
  const bool isWaitingToConvert(void) const;

  /**
   * @brief Check if the current media is waiting to validate.
   *
   * @return true if the media activity is WAITING_VALIDATE.
   */
  const bool isWaitingToValidate(void) const;

  /**
   * @brief Get ms until item is completed based on its total frames,
   * current fps, and completed frames.
   *
   * @return const int
   */
  int eta() const;

  /**
   * @brief Get the percent of completed frames over total frames as a whole
   * number.
   *
   * @return const int
   */
  int percentCompleted() const;

  /**
   * @brief Get how close the CR value is to the specified one.
   *
   * @return const float
   */
  float quality() const;

  /**
   * @brief Get how many frames the video is encoding over the source's FPS.
   *
   * @return const float
   */
  float speed() const;

  /**
   * @brief Get how much the encoding process has reduced the file size as a
   * whole number.
   *
   * @return float
   */
  int percentReduced() const;

  FileContainer &getFile() { return this->file; }

  ChildOptions &getOptions();

  std::string convertingLine() const;
  std::string pendingLine() const;

  void fromJSON(nlohmann::json) override;

  nlohmann::json toJSON(void) override;

private:
  /// @brief activity type
  Activity_N::Activity activity;
  FileContainer file;
  ChildOptions &childOptions; /// @brief owner child
};

#endif // !MEDIA
