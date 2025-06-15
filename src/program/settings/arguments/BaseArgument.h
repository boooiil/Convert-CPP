#ifndef BASE_ARGUMENT_H
#define BASE_ARGUMENT_H

#include <cassert>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "GenericArgument.h"

template <typename T>
class BaseArgument : public GenericArgument {
public:
  BaseArgument(std::string _helpMessage, std::string _flag,
    std::string _longFlag, T data)
    : GenericArgument(),
    helpMessage(_helpMessage),
    flag(_flag),
    longFlag(_longFlag),
    value(data) {
    LOG("Creating BaseArgument with flag: " + _flag +
      ", longFlag: " + _longFlag + ", helpMessage: " + _helpMessage);
    assert(_flag[0] == '-');
    assert(_longFlag[0] == '-');
  }

  ~BaseArgument(void) {}

  /**
   * @brief Parse a given argument.
   *
   * @param[in] argument  - Argument.
   */
  virtual void parse(std::string argument) = 0;
  virtual T get(void) { return value; };
  virtual void set(const T& provided) { value = provided; };

  virtual std::string getHelpMessage(void) { return helpMessage; };
  virtual std::string getLongFlag(void) { return longFlag; };
  virtual std::string getFlag(void) { return flag; };

  virtual bool isFlag(std::string _flag) {
    return this->flag == _flag || this->longFlag == _flag;
  };

  virtual operator T(void) const { return value; };

protected:
  /// @brief Help message for the argument.
  std::string helpMessage;
  std::string flag;
  std::string longFlag;
  /// @brief Value of provided argument.
  T value;
};

#endif  // BASE_ARGUMENT_H