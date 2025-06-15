#ifndef QUALITY_H
#define QUALITY_H

#include <string>

#include "../../../../utils/RegexUtils.h"
#include "../../../child/media/MediaDefinedFormat.h"
#include "../../../child/media/MediaFormat.h"
#include "../BaseArgument.h"

class Quality : public BaseArgument<MediaFormat> {
public:
  Quality(std::string _helpMessage, std::string _flag,
    std::string _longFlag, MediaFormat data) :
    BaseArgument(_helpMessage, _flag, _longFlag, data) {
  }

  ~Quality(void) {}

  void parse(std::string argument) override {
    // search if the provided string is a valid quality format
    if (MediaDefinedFormat::formats.contains(argument)) {
      this->value = MediaDefinedFormat::formats.at(argument);
    }
    else if (RegexUtils::isMatch(argument, "[0-9]+p")) {

      std::string numberPart = argument.substr(0, argument.size() - 1);

      // Check if all characters in numberPart are digits
      for (char c : numberPart) {
        if (!std::isdigit(c)) {
          throw std::invalid_argument("Invalid quality format: " + argument + ". Not all numbers.");
        }
      }

      MediaFormat custom = MediaFormat(argument, 24, 1.0f, 0.8f, 1.2f, 0, 0, std::string(""), std::string(""));
      custom.fromCustom(std::stoi(numberPart));
    }
    else {
      this->setErrored(true);
    }
  }

  const std::string toString(void) const override { return this->value.name; }
  const bool hasData(void) const override { return !this->value.name.empty(); }
};

#endif  // QUALITY_H