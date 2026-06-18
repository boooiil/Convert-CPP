#include "PlatformInfo.h"
#include "src/utils/logging/Logger.h"

const PlatformInfo PlatformInfo::detect() {
  bool is_x64 = sizeof(void *) == 8 ? true : false;
  std::string arch_str = is_x64 ? "64-bit" : "32-bit";
#ifdef _WIN32
  LOG_DEBUG(Logger::Priority::INFO, "Detected platform Windows (", arch_str,
            ").");
  return {.platform = Platform_N::WINDOWS, .x64_supported = is_x64};
#elif __APPLE__
  LOG_DEBUG(Logger::Priority::INFO, "Detected platform MacOS (", arch_str,
            ").");
  return {.platform = Platform_N::MACOS, .x64_supported = is_x64};
#elif __linux__
  LOG_DEBUG(Logger::Priority::INFO, "Detected platform Linux (", arch_str,
            ").");
  return {.platform = Platform_N::_LINUX, .x64_supported = is_x64};
#else
  LOG_DEBUG(Logger::Priority::INFO, "Unknown platform detected. (", arch_str,
            ").");
  return {.platform = Platform_N::INVALID, .x64_supported = is_x64};
#endif
}

const nlohmann::json PlatformInfo::toJSON() const {
  nlohmann::json platformInfo;

  platformInfo["platform"] = Platform_N::definition(this->platform);
  platformInfo["x64_supported"] = this->x64_supported;

  return platformInfo;
}