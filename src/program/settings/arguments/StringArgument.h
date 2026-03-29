#ifndef STRING_ARGUMENT_H
#define STRING_ARGUMENT_H

#include <string>

#include "BaseArgument.h"

class StringArgument : public BaseArgument<std::string> {
public:
  StringArgument(std::string _helpMessage, std::string _flag,
                 std::string _longFlag, std::string _typeDescriptor,
                 std::string data)
      : BaseArgument<std::string>(_helpMessage, _flag, _longFlag,
                                  _typeDescriptor, data) {}

  ~StringArgument() {}

  void parse(std::string argument) override { value = argument; }

  const std::string toString(void) const override { return value; }
  const bool hasData(void) const override { return !value.empty(); }
};

#endif // STRING_ARGUMENT_H