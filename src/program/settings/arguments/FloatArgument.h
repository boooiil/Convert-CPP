#ifndef FLOAT_ARGUMENT_H
#define FLOAT_ARGUMENT_H

#include "BaseArgument.h"

class FloatArgument : public BaseArgument<float> {
 public:
  operator std::string(void) const { return std::to_string(value); };
  operator const char*(void) const { return std::to_string(value).c_str(); };

  std::string asString(void) { return std::to_string(value); };
  const char* c_str(void) { return asString().c_str(); };
};

#endif  // FLOAT_ARGUMENT_H