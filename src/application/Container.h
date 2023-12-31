/*********************************************************************
 * @file   Container.h
 * @brief  Container class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef CONTAINER
#define CONTAINER

#include <queue>

#include "../application/Settings.h"
#include "../application/UserArguments.h"
#include "../application/UserCapabilities.h"
#include "../logging/Log.h"
#include "../media/Media.h"
#include "ApplicationEncodingDecision.h"

/**
 * @brief Holds all media files necessary for running the application.
 * @brief Also holds application setttings and user arguments.
 */
class Container {
 public:
  Container(void);
  ~Container(void);

  Log log;

  ApplicationEncodingDecision appEncodingDecision;
  Settings settings;
  UserArguments userArguments;
  UserCapabilities userCapabilities;

  /// @brief Holds current media file(s) being converted.
  std::queue<Media> converting;
  /// @brief Holds media files that are waiting to be converted.
  std::queue<Media> pending;

  /**
   * @brief Scans working directory for media files and adds them to pending
   * queue.
   *
   */
  void scanWorkingDir(void);
};

#endif  // !CONTAINER
