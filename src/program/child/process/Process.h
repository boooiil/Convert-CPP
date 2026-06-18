#ifndef F15898B7_DD0B_4E52_969A_8164EB05FA04
#define F15898B7_DD0B_4E52_969A_8164EB05FA04

#include <string>

class Process {
public:
  Process(const std::string &command)
      : command(command), pid(-1), state(WAITING) {};
  ~Process();

  enum STATES { INVALID = 0, RUNNING, WAITING, STOPPED, ERRORED };

  void start() {};
  void stop();
  void pause();
  void resume();

  const bool isRunning() const;
  const bool isStopped() const;
  const bool isPaused() const;
  const bool isErrored() const;

  const int getPid() const;
  const STATES getState() const;

private:
  const std::string command; // Command used to start the process
  int pid;                   // Process ID
  STATES state;              // Current state of the process
};

#endif /* F15898B7_DD0B_4E52_969A_8164EB05FA04 */
