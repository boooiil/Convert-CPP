#ifndef PROGRAM_H
#define PROGRAM_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "context/Arguments.h"
#include "context/RuntimeEnvironment.h"
#include "generics/JSONSerializable.h"
#include "generics/JSONSerializableRunner.h"

class Program : public JSONSerializable {
public:
  // static Log* log;
  static bool stopFlag;

  Program(void);
  ~Program(void);

  void prepare(std::vector<std::string> &args);
  void prepare(int argc, char *argv[]);
  void run(void);
  void end(void);

  void setEndable(bool);
  bool isEndable(void);

  Arguments &getArguments() { return *this->arguments; }

  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON(void) override;

private:
  bool endable;
  RuntimeEnvironment *runtimeEnv;
  JSONSerializableRunner *ticker;
  std::shared_ptr<Arguments> arguments;
};

#endif // !PROGRAM_H