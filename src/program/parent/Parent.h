#ifndef PARENT_H
#define PARENT_H

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <queue>
#include <vector>

#include "../child/Child.h"
#include "../generics/JSONSerializableRunner.h"
#include "src/program/context/Arguments.h"
#include "src/program/context/RuntimeEnvironment.h"

class Parent : public JSONSerializableRunner {
public:
  Parent(RuntimeEnvironment &run_env, Arguments &arguments);
  ~Parent(void);

  /// @brief Current converting child processes.
  std::queue<Child *> converting;
  /// @brief Pending child processes.
  std::queue<Child *> pending;
  // void prepare(ArgumentParser* arguments);
  /// @brief Run preparation tasks for parent.
  void prepare(std::vector<std::string> &args) override;
  ///  @brief Run parent process.
  void run(void) override;
  ///  @brief End parent process.
  void end(void) override;

  Arguments &getArguments();

  void fromJSON(const nlohmann::json &json) override;

  /**
   * @brief Returns the JSON representation of this object.
   *
   * @return nlohmann::json
   */
  nlohmann::json toJSON(void) override;

  std::vector<std::string> getArgs(std::filesystem::directory_entry file);

private:
  bool endable;
  RuntimeEnvironment &run_env;
  Arguments &arguments;
};

#endif // !PARENT_H