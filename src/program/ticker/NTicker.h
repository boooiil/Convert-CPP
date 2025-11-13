#ifndef NTICKER_H
#define NTICKER_H

#include <nlohmann/json_fwd.hpp>

#include "../generics/JSONSerializableRunner.h"
#include "src/program/generics/BaseDisplay.h"
#include <string>
#include <vector>

class NTicker : public JSONSerializableRunner {
public:
  JSONSerializableRunner *runner;

  NTicker(void);
  void determineNextAction(std::vector<std::string> &args);
  void prepare(std::vector<std::string> &args);
  // void prepare(ArgumentParser* arguments);
  void run(void);
  void end(void);

  void fromJSON(nlohmann::json);
  nlohmann::json toJSON(void);

private:
  bool endable;
  BaseDisplay *display;
  void writeDebug(void);
};

#endif // !NTICKER_H