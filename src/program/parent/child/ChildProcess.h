#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "../../child/media/Media.h"
#include "../../settings/enums/Activity_N.h"

class ChildProcess : public Media {
public:
  ChildProcess(std::string path, std::string filename);
  ~ChildProcess(void);

  int pid;
  long started;
  long ended;

  std::string path;
  std::string filename;
  std::string args;

  void getArgs(void);

  void doConversion(void);

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
  const bool isProcessing(void);

  /**
   * @brief Check if the current media has failed processing.
   *
   * @return true if the media activity is FAILED-like.
   */
  const bool hasFailed(void);

  /**
   * @brief Check if the current media has failed processing.
   *
   * @return true if the media activity is FINISHED.
   */
  const bool hasFinished(void);

  /**
   * @brief Check if the current media is waiting to be processed.
   *
   * @return true if the media activity is WAITING.
   */
  const bool isWaiting(void);

  /**
   * @brief Check if the current media is waiting to convert.
   *
   * @return true if the media activity is WAITING_CONVERT.
   */
  const bool isWaitingToConvert(void);

  void fromJSON(nlohmann::json json) override;

  /**
   * @brief Returns the process as a JSON object.
   *
   * @return nlohmann::json
   */
  nlohmann::json toJSON(void) override;

private:
  Activity_N::Activity activity;
  bool endable;
};

#endif // !CHILD_PROCESS_H
