#ifndef ENUM_ARGUMENT_H
#define ENUM_ARGUMENT_H

#include <string>
#include <type_traits>

#include "../../../utils/logging/Logger.h"
#include "../enums/EnumToStringFactory.h"
#include "BaseArgument.h"

template <EnumType T>
class EnumArgument : public BaseArgument<T> {
 public:
  EnumArgument(std::string _helpMessage, std::string _flag,
               std::string _longFlag, T data)
      : BaseArgument<T>(_helpMessage, _flag, _longFlag, data) {};

  ~EnumArgument(void) {};

  void parse(std::string argument) override {
    if (argument.empty()) {
      LOG("argument was empty");
      this->setErrored(true);
      return;
    }

    this->value = EnumToStringFactory::fromName<T>(argument);
  }

  virtual const std::string toString(void) const override {
    StringEnumDataHolder s_data = EnumToStringFactory::get(this->value);
    return s_data.getName();
  }

  virtual const bool hasData(void) const override {
    return static_cast<int>(this->value) != 0;
  };
};
#endif  // !ENUM_ARGUMENT_H