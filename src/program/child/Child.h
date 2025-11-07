#ifndef CHILD_H
#define CHILD_H

#include <nlohmann/json_fwd.hpp>
#include <queue>
#include <string>
#include <uuid.h>
#include <vector>

#include "../generics/JSONSerializableRunner.h"
#include "./media/Media.h"

class Child : public JSONSerializableRunner {
public:
  uuids::uuid id;
  /// @brief Holds current media files being converted.
  std::queue<Media *> converting;
  /// @brief Holds media files that are waiting to be converted.
  std::queue<Media *> pending;

  Child(void);

  /// @brief Prepares the child for conversion.
  void prepare(std::vector<std::string> &args) override;
  ///  @brief Runs the child's conversion process.
  void run(void) override;
  /// @brief Ends the child's conversion process.
  void end(void) override;

  void fromJSON(nlohmann::json) override;

  nlohmann::json toJSON(void) override;

private:
  bool endable;
  bool completed;
};

#endif // !CHILD_H
