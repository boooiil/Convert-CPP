#ifndef ENUM_ARGUMENT_H
#define ENUM_ARGUMENT_H

#include <string>
#include <type_traits>

#include "../../../utils/logging/Logger.h"
#include "BaseArgument.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/utils/logging/Logger.h"

template <typename T>
  requires std::is_enum_v<T>
class EnumArgument : public BaseArgument<T> {
public:
  EnumArgument(std::string _helpMessage, std::string _flag,
               std::string _longFlag, T data)
      : BaseArgument<T>(_helpMessage, _flag, _longFlag, data) {};

  ~EnumArgument(void) {};

  void parse(std::string argument) override {
    if (argument.empty()) {
      LOG_DEBUG("argument was empty");
      this->setErrored(true);
      return;
    }
    LOG_DEBUG("Trying to get enum name: " + argument);
    this->value = DefinitionRegistry::enumFromDefName<T>(argument);
  }

  virtual const std::string toString(void) const override {
    return DefinitionRegistry::defFromEnum(this->value);
  }

  virtual const bool hasData(void) const override {
    return static_cast<int>(this->value) != 0;
  };
};
#endif // !ENUM_ARGUMENT_H