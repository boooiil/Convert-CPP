#ifndef PARENT_OPTIONS_H
#define PARENT_OPTIONS_H

#include <nlohmann/json_fwd.hpp>

#include "../../generics/JSONSerializable.h"
#include "../arguments/ArgumentRegistry.h"

class ParentOptions : public JSONSerializable {
 public:
  ParentOptions(void);
  ~ParentOptions(void);

  // TODO dtor deletion
  ArgumentRegistry* argumentRegistry;
  std::vector<std::string> i_args;

  void prepare(void);
  void parse(std::vector<std::string>& args);
  void validate(void);
  void invalidArgument(std::string arg);

  void fromJSON(const nlohmann::json json) override;
  nlohmann::json toJSON(void);
};
#endif  // !PARENT_OPTIONS_H
