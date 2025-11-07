#include "ChildProcessConversion.h"

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "../../child/media/MediaProcess.h"
#include "ChildProcess.h"

ChildProcessConversion::ChildProcessConversion(ChildProcess *childProcess)
    : MediaProcess(childProcess) {}

void ChildProcessConversion::parse(std::string data) {
  LOG_DEBUG("Parsing data.", data);

  data.erase(std::remove(data.begin(), data.end(), '\f'), data.end());

  try {
    nlohmann::json json = nlohmann::json::parse(data);
    this->object->fromJSON(json);
  } catch (nlohmann::json::parse_error &e) {
    throw e.what();
  }
}
