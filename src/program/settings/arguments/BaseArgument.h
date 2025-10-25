#ifndef BASE_ARGUMENT_H
#define BASE_ARGUMENT_H

#include <cassert>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "GenericArgument.h"

template <typename T> class BaseArgument : public GenericArgument {
public:
  BaseArgument(std::string _helpMessage, std::string _flag,
               std::string _longFlag, T data)
      : GenericArgument(), helpMessage(_helpMessage), flag(_flag),
        longFlag(_longFlag), value(data) {
    LOG_DEBUG("Creating BaseArgument with flag: " + _flag +
              ", longFlag: " + _longFlag + ", helpMessage: " + _helpMessage);
    assert(_flag[0] == '-');
    assert(_longFlag[0] == '-');
  }

  ~BaseArgument(void) {}

  virtual T get(void) { return value; };
  virtual void set(const T &provided) { value = provided; };

  virtual std::string getHelpMessage(void) override { return helpMessage; };
  virtual std::string getLongFlag(void) override { return longFlag; };
  virtual std::string getFlag(void) override { return flag; };

  virtual operator T(void) const { return value; };

protected:
  /// @brief Help message for the argument.
  std::string helpMessage;
  std::string flag;
  std::string longFlag;
  /// @brief Value of provided argument.
  T value;
};

#endif // BASE_ARGUMENT_H