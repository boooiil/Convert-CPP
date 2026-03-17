/*********************************************************************
 * @file   ExecuteProcess.h
 * @brief  ???
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef EXECUTE_PROCESS
#define EXECUTE_PROCESS

#include <string>

/**
 * @brief ???
 */
class ExecuteProcess {
 public:
  ExecuteProcess();
  ~ExecuteProcess();

  /// @brief Status of the process.
  enum Status { WAIT, DONE, RUNNING, ERROR };

  /**
   * @brief Start a process.
   *
   * @param[in] command - command to execute.
   */
  void start(std::string command);

  /**
   * @brief Set the status of the process.
   *
   * @param[in] status - status of the process.
   */
  void setStatus(Status status);

  /**
   * @brief Stop the process.
   *
   * @return true if the process is stopped.
   */
  bool stop(void);

  /**
   * @brief Check if the process is waiting.
   *
   * @return true if the process is waiting.
   */
  bool isWaiting(void) const;

  /**
   * @brief Check if the process is running.
   *
   * @return true if the process is running.
   */
  bool isRunning(void) const;

  /**
   * @brief Check if the process is errored.
   *
   * @return true if the process is errored.
   */
  bool isError(void) const;

  /**
   * @brief Check if the process is done.
   *
   * @return true if the process is done.
   */
  bool isDone(void) const;

  /**
   * @brief Get the status of the process.
   *
   * @return Status of the process.
   */
  Status getStatus(void) const;

 private:
  /// @brief Status of the process.
  Status status;
};

#endif  //! EXECUTE_PROCESS