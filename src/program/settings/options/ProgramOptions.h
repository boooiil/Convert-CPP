#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

// #include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <regex>
#include <string>
#include <vector>

#include "../../settings/enums/Tunes_N.h"
#include "src/program/registry/ArgumentRegistry.h"

class ProgramOptions : public JSONSerializable {
public:
  ProgramOptions(void);
  ~ProgramOptions(void);

  // TODO dtor deletion
  ArgumentRegistry *argumentRegistry;
  std::vector<std::string> i_args;

  /// @brief List of regular expressions matching tunes.
  /// @brief The matching tunes can be used as an index
  /// @brief in the tuneAssociations list.
  std::vector<std::regex> tuneRegex;
  /// @brief List of tunes.
  std::vector<Tunes_N::Tunes> tuneAssociations;

  void prepare(void);
  void parse(std::vector<std::string> args);
  void validate(void);
  void gatherSystemDetails(void);

  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON(void) override;
};

#endif // PROGRAM_SETTINGS_H