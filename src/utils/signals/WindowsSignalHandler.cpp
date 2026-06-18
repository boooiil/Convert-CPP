#ifdef _WIN32
#include "WindowsSignalHandler.h"

#include <Windows.h>
#include <consoleapi.h>

#include "../../program/Program.h"
#include "../../utils/logging/Logger.h"

WindowsSignalHandler::WindowsSignalHandler() {}
WindowsSignalHandler::~WindowsSignalHandler() {}

void WindowsSignalHandler::registerHandler() {
  /*LOG(Logger::Priority::INFO,"Registering signal handler.");
  if (!SetConsoleCtrlHandler(WindowsSignalHandler::winHandle, TRUE)) {
    LOG(Logger::Priority::INFO,"Failed to register signal handler.");
  }
  LOG(Logger::Priority::INFO,"Signal handler registered.");*/
}

BOOL WINAPI WindowsSignalHandler::winHandle(DWORD signal) {
  LOG(Logger::Priority::INFO, "Signal received.");
  switch (signal) {
  case CTRL_BREAK_EVENT:
  case CTRL_C_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_LOGOFF_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    LOG(Logger::Priority::INFO, "Interrupt signal received.");
    Program::stopFlag = true;
    return true;
  default:
    return false;
  }
}
#endif