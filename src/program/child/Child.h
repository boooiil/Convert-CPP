#ifndef CHILD_H
#define CHILD_H

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <queue>
#include <string>
#include <uuid.h>
#include <vector>

#include "../generics/JSONSerializableRunner.h"
#include "./media/Media.h"
#include "src/program/context/Arguments.h"
#include "src/program/context/RuntimeEnvironment.h"

class Child : public JSONSerializableRunner {
public:
  uuids::uuid id;
  /// @brief Holds current media files being converted.
  std::queue<Media *> converting;
  /// @brief Holds media files that are waiting to be converted.
  std::queue<Media *> pending;

  Child(RuntimeEnvironment &run_env, std::shared_ptr<Arguments> arguments);

  /// @brief Prepares the child for conversion.
  void prepare(std::vector<std::string> &args) override;
  ///  @brief Runs the child's conversion process.
  void run(void) override;
  /// @brief Ends the child's conversion process.
  void end(void) override;

  Arguments &getArguments() { return *this->arguments; }

  void fromJSON(const nlohmann::json &json) override;

  nlohmann::json toJSON(void) override;

private:
  bool endable;
  bool completed;
  RuntimeEnvironment &run_env;
  std::shared_ptr<Arguments> arguments;
};

#endif // !CHILD_H
