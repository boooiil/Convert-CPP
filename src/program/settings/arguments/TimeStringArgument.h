#ifndef TIME_STRING_ARGUMENT_H
#define TIME_STRING_ARGUMENT_H

#include <regex>
#include <string>

#include "../../../utils/RegexUtils.h"
#include "StringArgument.h"

class TimeStringArgument : public StringArgument {
public:
  TimeStringArgument(std::string _helpMessage, std::string _flag,
                     std::string _longFlag, std::string _typeDescriptor,
                     std::string data)
      : StringArgument(_helpMessage, _flag, _longFlag, _typeDescriptor, data) {}

  void parse(std::string argument) override {
    if (RegexUtils::isMatch(
            argument, R"(\d+(?:\.\d+|\:\d{2}\:\d{2}(?:\.\d+)?|(?:ms|ns|us)?))",
            std::regex::icase)) {
      this->value = argument;
    } else {
      this->setErrored(true);
    }
  };
};

#endif // TIME_STRING_ARGUMENT_H