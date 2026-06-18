#include "RuntimeEnvironment.h"
#include "HardwareCapabilities.h"
#include "src/utils/logging/Logger.h"

RuntimeEnvironment *RuntimeEnvironment::detect() {

  LOG_DEBUG(Logger::Priority::INFO, "Gathering system details...");
  return new RuntimeEnvironment{.hw_cap = HardwareCapabilities::detect(),
                                .platform_info = PlatformInfo::detect()};
}

nlohmann::json RuntimeEnvironment::toJSON() const {
  nlohmann::json runtimeEnv;

  runtimeEnv["hardware_capabilities"] = this->hw_cap.toJSON();
  runtimeEnv["platform_info"] = this->platform_info.toJSON();

  return runtimeEnv;
}