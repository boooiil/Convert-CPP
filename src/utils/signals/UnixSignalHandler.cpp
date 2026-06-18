#if defined(__unix__) || defined(__linux__)

#include "UnixSignalHandler.h"

#include <signal.h>

#include <iostream>

#include "../../program/Program.h"
#include "../../utils/logging/Logger.h"

UnixSignalHandler::UnixSignalHandler() {}
UnixSignalHandler::~UnixSignalHandler() {}

void UnixSignalHandler::registerHandler() {
  LOG(Logger::Priority::INFO, "Registering signal handler.");
  signal(SIGKILL, handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGINT, handleSignal);
  signal(SIGQUIT, handleSignal);
  LOG(Logger::Priority::INFO, "Signal handler registered.");
}

void UnixSignalHandler::handleSignal(int signal) {
  std::cout << "Interrupt signal (" << signal << ") recieved." << std::endl;
  Program::stopFlag = true;
}
#endif