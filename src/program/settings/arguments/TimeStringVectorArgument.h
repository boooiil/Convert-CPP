#ifndef TIME_STRING_VECTOR_ARGUMENT_H
#define TIME_STRING_VECTOR_ARGUMENT_H

#include <regex>
#include <string>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/RegexUtils.h"
#include "VectorArgument.h"

class TimeStringVectorArgument : public VectorArgument<std::string> {
public:
  TimeStringVectorArgument(std::string _helpMessage, std::string _flag,
                           std::string _longFlag, std::string _typeDescriptor,
                           std::vector<std::string> data)
      : VectorArgument<std::string>(_helpMessage, _flag, _longFlag,
                                    _typeDescriptor, data) {}

  void parse(std::string argument) override {
    std::vector<std::string> timestamps = ListUtils::splitv(argument, ",");

    if (timestamps.size() != 2) {
      this->setErrored(true);
      return;
    }

    bool first = RegexUtils::isMatch(
        timestamps[0], R"(\d+(?:\.\d+|\:\d{2}\:\d{2}(?:\.\d+)?|(?:ms|ns|us)?))",
        std::regex::icase);
    bool second = RegexUtils::isMatch(
        timestamps[1], R"(\d+(?:\.\d+|\:\d{2}\:\d{2}(?:\.\d+)?|(?:ms|ns|us)?))",
        std::regex::icase);

    if (first && second) {
      this->value = timestamps;
    } else {
      this->setErrored(true);
    }
  }
};

#endif // TIME_STRING_VECTOR_ARGUMENT_H