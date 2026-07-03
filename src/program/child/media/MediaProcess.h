/*********************************************************************
 * @file   MediaProcess.h
 * @brief  Media Process class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_PROCESS
#define MEDIA_PROCESS

#include "src/program/Program.h"
#include <stdio.h>

#include <memory>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <cstdio>
#define popen _popen
#define pclose _pclose
#else
#include <cstdio>
#endif
#include "../../../utils/logging/Logger.h"

/**
 * @brief Handle execution of commands on the system and track their status.
 */
template <typename T> class MediaProcess {
public:
  /**
   * @brief Construct a new Media Process object.
   */
  MediaProcess(T *_object)
      : object(_object), status(MediaProcess::Status::WAIT) {}

  ~MediaProcess() { LOG_DEBUG(Logger::Priority::INFO, "DESTRUCTOR CALLED"); }

  /**
   * @brief Status of the process.
   */
  enum Status { WAIT, DONE, RUNNING, _ERROR };

  /**
   * @brief Start the process.
   *
   * @param[in] command - Command to be executed.
   */
  virtual void start(std::string command) {
    command += " 2>&1";
    LOG_DEBUG(Logger::Priority::INFO, "SENDING COMMAND:", command);

    this->status = MediaProcess::Status::RUNNING;

    std::string result;
    int ch;

    // Open pipe to file
    auto pipe_deleter = [](FILE *f) { pclose(f); };
    std::unique_ptr<FILE, decltype(pipe_deleter)> pipe(
        popen(command.c_str(), "r"), pipe_deleter);

    if (!pipe) {
      throw std::runtime_error("popen() failed!");
    }

    if (Program::stopFlag) {
      LOG_DEBUG(Logger::Priority::INFO,
                "Stop request received before starting media process loop");
      return;
    }

    // Read from pipe
    while ((ch = fgetc(pipe.get())) != EOF && !Program::stopFlag) {
      result += static_cast<char>(ch);

      // If the last character is a newline, parse the result
      if (ch == 10 || ch == 13) {
        // parse the data using overriden func
        parse(result);
        result.clear(); // Clear the result for the next line
      }

      if (Program::stopFlag) {
        LOG_DEBUG(Logger::Priority::INFO,
                  "Stop request received during the loop");
        break;
      }
    }
  }

  /**
   * @brief Parse the data from the process.
   *
   * @param[in] data - Data to be parsed.
   */
  virtual void parse(std::string data) {
    LOG_DEBUG(Logger::Priority::INFO, "WARNING, USING DEFAULT PARSER", data);
  }

  /**
   * @brief Set the status of the process.
   *
   * @param[in]  - Status to be set.
   */
  void setStatus(MediaProcess::Status provided_status) {
    this->status = provided_status;
  }

  /**
   * @brief Check if the process is waiting.
   *
   * @return true  - Process is waiting.
   * @return false - Process is not waiting.
   */
  bool isWaiting(void) const {
    return this->status == MediaProcess::Status::WAIT;
  }

  /**
   * @brief Check if the process is running.
   *
   * @return true  - Process is running.
   * @return false - Process is not running.
   */
  bool isRunning(void) const {
    return this->status == MediaProcess::Status::RUNNING;
  }

  /**
   * @brief Check if the process is in error.
   *
   * @return true  - Process is in error.
   * @return false - Process is not in error.
   */
  bool isError(void) const {
    return this->status == MediaProcess::Status::_ERROR;
  }

  /**
   * @brief Check if the process is done.
   *
   * @return true  - Process is done.
   * @return false - Process is not done.
   */
  bool isDone(void) const { return this->status == MediaProcess::Status::DONE; }

  /**
   * @brief Get the status of the process.
   *
   * @return Status - Status of the process.
   */
  Status getStatus(void) const { return this->status; }

protected:
  T *object;
  Status status; /// @brief Status of the process.

private:
};

#endif // !MEDIA_PROCESS
