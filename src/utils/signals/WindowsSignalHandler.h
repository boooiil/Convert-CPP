#ifdef _WIN32
#ifndef WINDOWS_SIGNAL_HANDLER_H
#define WINDOWS_SIGNAL_HANDLER_H

#include <Windows.h>

class WindowsSignalHandler {
 public:
  WindowsSignalHandler();
  ~WindowsSignalHandler();

  void registerHandler();
  void handleSignal();

 private:
  static BOOL WINAPI winHandle(DWORD signal);
};

#endif  // WINDOWS_SIGNAL_HANDLER_H
#endif
