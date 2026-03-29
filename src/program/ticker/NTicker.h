#ifndef NTICKER_H
#define NTICKER_H

#include <nlohmann/json_fwd.hpp>

#include "../generics/JSONSerializableRunner.h"
#include "src/program/context/Arguments.h"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/generics/BaseDisplay.h"
#include <string>
#include <vector>

class NTicker : public JSONSerializableRunner {
public:
  JSONSerializableRunner *runner;

  NTicker(RuntimeEnvironment &run_env, std::shared_ptr<Arguments> &arguments);
  void determineNextAction(std::vector<std::string> &args);
  void prepare(std::vector<std::string> &args);
  // void prepare(ArgumentParser* arguments);
  void run(void);
  void end(void);

  void fromJSON(const nlohmann::json &json);
  nlohmann::json toJSON(void);

private:
  bool endable;
  BaseDisplay *display;
  std::shared_ptr<Arguments> arguments;
  RuntimeEnvironment &run_env;
  void writeDebug(void);
};

#endif // !NTICKER_H