#ifndef CHILD_OPTIONS_H
#define CHILD_OPTIONS_H

#include <string>

#include "../../generics/JSONSerializable.h"
#include "../enums/Encoders_N.h"
#include "../enums/HWAccelerators_N.h"
#include "nlohmann/json_fwd.hpp"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/registry/ArgumentRegistry.h"

class ChildOptions : public JSONSerializable {
public:
  ChildOptions(RuntimeEnvironment &runtimeEnvironment);
  ~ChildOptions(void);

  Encoders_N::Encoders runningEncoder;
  HWAccelerators_N::HWAccelerators runningHWAccel;

  // TODO dtor deletion
  ArgumentRegistry *argumentRegistry;
  std::vector<std::string> i_args;
  std::string CWD;

  void prepare(void);
  void parse(std::vector<std::string> &args);
  void validate(void);

  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON(void) override;

private:
  RuntimeEnvironment &run_env;
};

#endif // !CHILD_OPTIONS_H
