#ifndef CHILD_H
#define CHILD_H

#include <nlohmann/json_fwd.hpp>
#include <queue>
#include <string>
#include <uuid.h>
#include <vector>

#include "../generics/JSONSerializableRunner.h"
#include "./media/Media.h"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/settings/options/ChildOptions.h"

class Child : public JSONSerializableRunner {
public:
  uuids::uuid id;
  /// @brief Holds current media files being converted.
  std::queue<Media *> converting;
  /// @brief Holds media files that are waiting to be converted.
  std::queue<Media *> pending;

  Child(RuntimeEnvironment &run_env);

  /// @brief Prepares the child for conversion.
  void prepare(std::vector<std::string> &args) override;
  ///  @brief Runs the child's conversion process.
  void run(void) override;
  /// @brief Ends the child's conversion process.
  void end(void) override;

  ChildOptions &getOptions() { return *this->childOptions; }

  void fromJSON(const nlohmann::json &json) override;

  nlohmann::json toJSON(void) override;

private:
  bool endable;
  bool completed;
  RuntimeEnvironment &run_env;
  ChildOptions *childOptions; // no delete, owned by Program::settings
};

#endif // !CHILD_H
