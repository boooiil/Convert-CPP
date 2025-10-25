#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

// #include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <regex>
#include <string>
#include <vector>

#include "../../settings/enums/Encoders.h"
#include "../../settings/enums/HWAccelerators.h"
#include "../../settings/enums/Tunes.h"
#include "../enums/GPUProviders.h"
#include "../enums/Platform.h"
#include "src/program/registry/ArgumentRegistry.h"

class ProgramOptions : public JSONSerializable {
public:
  ProgramOptions(void);
  ~ProgramOptions(void);

  // TODO dtor deletion
  ArgumentRegistry *argumentRegistry;
  std::vector<std::string> i_args;

  /// @brief The current operating system of the user's machine
  Platform platform;
  /// @brief List of supported encoders by the program.
  std::vector<Encoders> supportedEncoders;
  /// @brief List of supported hardware accelerators by the program.
  std::vector<HWAccelerators> supportedHWAccel;
  /// @brief List of regular expressions matching tunes.
  /// @brief The matching tunes can be used as an index
  /// @brief in the tuneAssociations list.
  std::vector<std::regex> tuneRegex;
  /// @brief List of tunes.
  std::vector<Tunes> tuneAssociations;

  // TODO: change this to a map vvvv
  /// @brief The current GPU of the user's machine
  std::vector<GPUProviders> GPU_Providers;
  /// @brief The preferred GPU provider for the user.
  GPUProviders preferredGPUProvider;

  void prepare(void);
  void parse(std::vector<std::string> args);
  void validate(void);
  void gatherSystemDetails(void);

  void fromJSON(nlohmann::json) override;
  nlohmann::json toJSON(void) override;
};

#endif // PROGRAM_SETTINGS_H