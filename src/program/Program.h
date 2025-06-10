#ifndef PROGRAM_H
#define PROGRAM_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../utils/logging/Logger.h"
#include "generics/JSONSerializable.h"
#include "generics/JSONSerializableRunner.h"
#include "settings/Settings.h"

class Program : public JSONSerializable {
public:
  static Settings* settings;
  // static Log* log;
  static JSONSerializableRunner* ticker;
  static bool stopFlag;

  Program(void);
  ~Program(void);

  void prepare(std::vector<std::string>& args);
  void prepare(int argc, char* argv[]);
  void run(void);
  void end(void);

  void setEndable(bool);
  bool isEndable(void);

  void fromJSON(nlohmann::json);
  nlohmann::json toJSON(void);

private:
  bool endable;
};

#endif  // !PROGRAM_H