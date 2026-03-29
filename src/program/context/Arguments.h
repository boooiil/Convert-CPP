#ifndef BB5BA0D3_C5C7_4CD1_A8B2_19D8591C55EC
#define BB5BA0D3_C5C7_4CD1_A8B2_19D8591C55EC

#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/generics/JSONSerializable.h"
#include "src/program/registry/ArgumentRegistry.h"
#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/program/settings/enums/Tunes_N.h"
#include <memory>

struct Arguments : JSONSerializable {
  Arguments(RuntimeEnvironment &run_env, const std::vector<std::string> &i_args,
            const std::string &CWD);
  ArgumentRegistry argumentRegistry;

  void validate();

  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON(void) override;

  static std::shared_ptr<Arguments> parse(RuntimeEnvironment &run_env,
                                          std::vector<std::string> &args);

  RuntimeEnvironment &run_env;

  // this will eventually be moved to Media
  Encoders_N::Encoders running_encoder;
  // this will eventually be moved to Media
  HWAccelerators_N::HWAccelerators running_hw_accel;

  const std::string &CWD;
  const std::vector<std::string> &i_args;

  // might not need this anymore
  std::vector<std::string> tune_regex_strings;
  // might not need this anymore
  std::vector<Tunes_N::Tunes> tune_associations;
};

#endif /* BB5BA0D3_C5C7_4CD1_A8B2_19D8591C55EC */
