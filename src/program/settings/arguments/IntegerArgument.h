#ifndef INTEGER_ARGUMENT_H
#define INTEGER_ARGUMENT_H

#include <stdexcept>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "BaseArgument.h"

class IntegerArgument : public BaseArgument<int> {
public:
  IntegerArgument(std::string _helpMessage, std::string _flag,
                  std::string _longFlag, std::string _typeDescriptor, int data)
      : BaseArgument<int>(_helpMessage, _flag, _longFlag, _typeDescriptor,
                          data) {}

  ~IntegerArgument() {}

  void parse(std::string argument) override {
    try {
      value = std::stoi(argument);
    } catch (std::invalid_argument &e) {
      LOG_DEBUG(Logger::Priority::INFO, e.what());
      this->setErrored(true);
    }
  }

  virtual const std::string toString(void) const override {
    return std::to_string(value);
  }
  virtual const bool hasData(void) const override { return this->value != -1; }
};

#endif // INTEGER_ARGUMENT_H