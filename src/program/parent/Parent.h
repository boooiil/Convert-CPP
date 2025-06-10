#ifndef PARENT_H
#define PARENT_H

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <queue>
#include <vector>

#include "../child/Child.h"
#include "../generics/JSONSerializableRunner.h"

class Parent : public JSONSerializableRunner {
public:
  ~Parent(void);

  /// @brief Current converting child processes.
  std::queue<Child*> converting;
  /// @brief Pending child processes.
  std::queue<Child*> pending;
  //void prepare(ArgumentParser* arguments);
  /// @brief Run preparation tasks for parent.
  void prepare(std::vector<std::string>& args);
  ///  @brief Run parent process.
  void run(void);
  ///  @brief End parent process.
  void end(void);

  void setEndable(bool endable);
  bool isEndable(void);

  void fromJSON(nlohmann::json);

  /**
   * @brief Returns the JSON representation of this object.
   *
   * @return nlohmann::json
   */
  nlohmann::json toJSON(void);

  std::vector<std::string> getArgs(std::filesystem::directory_entry file);

private:
  bool endable;
};

#endif  // !PARENT_H