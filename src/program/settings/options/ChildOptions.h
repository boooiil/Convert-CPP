#ifndef CHILD_OPTIONS_H
#define CHILD_OPTIONS_H

#include <string>

#include "../../generics/JSONSerializable.h"
#include "../arguments/ArgumentRegistry.h"
#include "../enums/Encoders.h"
#include "../enums/HWAccelerators.h"
#include "../enums/StringEnumDataHolder.h"
#include "nlohmann/json_fwd.hpp"

class ChildOptions : public JSONSerializable {
 public:
  ChildOptions(void);
  ~ChildOptions(void);

  Encoders runningEncoder;
  HWAccelerators runningHWAccel;

  // TODO dtor deletion
  ArgumentRegistry* argumentRegistry;
  std::vector<std::string> i_args;
  std::string CWD;

  void prepare(void);
  void parse(std::vector<std::string>& args);
  void validate(void);
  void invalidArgument(std::string);

  void fromJSON(const nlohmann::json json) override;
  nlohmann::json toJSON(void) override;
};

#endif  // !CHILD_OPTIONS_H
