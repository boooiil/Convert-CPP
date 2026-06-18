#ifndef GENERIC_RUNNER_H
#define GENERIC_RUNNER_H

#include "src/utils/logging/Logger.h"
#include <string>
#include <type_traits>
#include <vector>

class GenericRunner {
public:
  virtual void prepare(std::vector<std::string> &args) = 0;
  virtual void run(void) = 0;
  virtual void end(void) = 0;

  virtual bool isStopping(void) const { return endable; };
  virtual void setStopping(bool flag) { endable = flag; };

  virtual bool isCompleted(void) const { return completed; };
  virtual void setCompleted(bool flag) {
    LOG_DEBUG(Logger::Priority::INFO,
              "runner marked as completed :", flag ? "True" : "False");
    completed = flag;
  };

  template <typename T> T *getRunner(void) {
    static_assert(std::is_base_of<GenericRunner, T>::value,
                  "T must be a subclass of GenericRunner");
    return dynamic_cast<T *>(this);
  };

private:
  bool endable = true;
  bool completed = false;
};

#endif // !GENERIC_RUNNER_H
