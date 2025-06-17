#ifndef FLAG_ARGUMENT_H
#define FLAG_ARGUMENT_H

#include <string>

#include "BaseArgument.h"

/**
 * @brief A class that represents a flag argument with a BaseArgument of type
 * bool.
 *
 */
class FlagArgument : public BaseArgument<bool> {
 public:
  FlagArgument(std::string _helpMessage, std::string _flag,
               std::string _longFlag, bool data)
      : BaseArgument(_helpMessage, _flag, _longFlag, data) {}

  ~FlagArgument() {}

  void parse(std::string argument) override {
    if (argument == "true" || argument == "false") {
      value = argument == "true";
    } else {
      this->setErrored(true);
    }
  }

  virtual const std::string toString(void) const override {
    return value ? "true" : "false";
  }

  virtual const bool hasData(void) const override { return value; }

  explicit operator bool(void) const {
    std::cout << "operator bool() called, value: " << value << std::endl;
    return value;
  }
};

#endif  // FLAG_ARGUMENT_H