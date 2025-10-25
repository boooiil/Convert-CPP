#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <stdio.h>

#include <array>
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/RegexUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../arguments/EnumArgument.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "../enums/Encoders.h"
#include "../enums/GPUProviders.h"
#include "../enums/HWAccelerators.h"
#include "../enums/Platform.h"
#include "../enums/StringEnumDataHolder.h"
#include "../enums/Tunes.h"
#include "ProgramOptions.h"

// TODO: fill this out, rm ApplicationEncodingDecision

ProgramOptions::ProgramOptions()
    : argumentRegistry(new ArgumentRegistry()), platform(Platform::INVALID),
      GPU_Providers({}), preferredGPUProvider(GPUProviders::INVALID) {
  ProgramOptions::tuneRegex = {std::regex(R"(film)", std::regex::icase),
                               std::regex(R"(anim)", std::regex::icase),
                               std::regex(R"(grain)", std::regex::icase)};

  ProgramOptions::tuneAssociations = {Tunes::FILM, Tunes::ANIMATION,
                                      Tunes::GRAIN};
};

ProgramOptions::~ProgramOptions(void) {
  LOG_DEBUG("Destroying ProgramOptions...");
  LOG_DEBUG("Expecting to destroy { ArgumentRegistry }");
  if (this->argumentRegistry != nullptr) {
    delete this->argumentRegistry;
  }
};

void ProgramOptions::gatherSystemDetails(void) {
#ifdef _WIN32
  this->platform = Platform::WINDOWS;

  std::array<char, 128> buffer;
  std::string result;

  // Open pipe to file
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
      popen("wmic path win32_VideoController get name", "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  // Read from pipe
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }

  LOG_DEBUG("GPU Provider Raw:", result);

  // nvidia
  if (RegexUtils::isMatch(result, "nvidia", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::NVIDIA);
  }
  // intel
  if (RegexUtils::isMatch(result, "intel", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::INTEL);
  }
  // amd
  if (RegexUtils::isMatch(result, "amd", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::AMD);
  }
  // unknown
  if (this->GPU_Providers.empty()) {
    this->GPU_Providers.push_back(GPUProviders::INVALID);
  }
#elif __linux__
  platform = Platform::_LINUX;

  std::array<char, 128> buffer;
  std::string result;

  // Open pipe to file
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lspci | grep VGA", "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  // Read from pipe
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  LOG_DEBUG("GPU Provider Raw:", result);

  // nvidia
  if (RegexUtils::isMatch(result, "nvidia", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::NVIDIA);
  }
  // intel
  if (RegexUtils::isMatch(result, "intel", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::INTEL);
  }
  // amd
  if (RegexUtils::isMatch(result, "amd", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::AMD);
  }
  // unknown
  if (this->GPU_Providers.empty()) {
    this->GPU_Providers.push_back(GPUProviders::INVALID);
  }

#else
  throw std::runtime_error("Unsupported platform!");
#endif

  if (ListUtils::contains(this->GPU_Providers, GPUProviders::NVIDIA)) {
    this->preferredGPUProvider = GPUProviders::NVIDIA;
  } else if (ListUtils::contains(this->GPU_Providers, GPUProviders::INTEL)) {
    this->preferredGPUProvider = GPUProviders::INTEL;
  } else if (ListUtils::contains(this->GPU_Providers, GPUProviders::AMD)) {
    this->preferredGPUProvider = GPUProviders::AMD;
  } else {
    this->preferredGPUProvider = GPUProviders::INVALID;
  }

  LOG_DEBUG("Platform:", EnumToStringFactory::get(this->platform).getName());

  LOG_DEBUG("Preferred GPU Provider:",
            EnumToStringFactory::get(this->preferredGPUProvider).getName());

  if (ListUtils::contains(this->GPU_Providers, GPUProviders::NVIDIA)) {
    // set encoders
    supportedEncoders.push_back(Encoders::AV1_NVENC);
    supportedEncoders.push_back(Encoders::H264_NVENC);
    supportedEncoders.push_back(Encoders::HEVC_NVENC);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_CUVID);
    // supportedDecoders.push_back(Decoders::H264_CUVID);
    // supportedDecoders.push_back(Decoders::HEVC_CUVID);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators::NVIDIA);
  }
  if (ListUtils::contains(this->GPU_Providers, GPUProviders::INTEL)) {
    // set encoders
    supportedEncoders.push_back(Encoders::AV1_QSV);
    supportedEncoders.push_back(Encoders::H264_QSV);
    supportedEncoders.push_back(Encoders::HEVC_QSV);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_QSV);
    // supportedDecoders.push_back(Decoders::H264_QSV);
    // supportedDecoders.push_back(Decoders::HEVC_QSV);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators::INTEL);
  }
  if (ListUtils::contains(this->GPU_Providers, GPUProviders::AMD)) {
    // set encoders
    supportedEncoders.push_back(Encoders::AV1_AMF);
    supportedEncoders.push_back(Encoders::H264_AMF);
    supportedEncoders.push_back(Encoders::HEVC_AMF);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators::AMD);
  }

  // set software encoders
  supportedEncoders.push_back(Encoders::AV1);
  supportedEncoders.push_back(Encoders::H264);
  supportedEncoders.push_back(Encoders::HEVC);

  for (Encoders encoder : supportedEncoders) {
    LOG_DEBUG("Supported Encoder:",
              EnumToStringFactory::get(encoder).getName());
  }

  for (HWAccelerators hwAccel : supportedHWAccel) {
    LOG_DEBUG("Supported HW Accel:",
              EnumToStringFactory::get(hwAccel).getName());
  }

  // for (Decoders::Codec decoder : supportedDecoders) {
  //   LOG_DEBUG("Supported Decoder:",
  //       Decoders::getValue(decoder));
  // }
}

void ProgramOptions::prepare(void) {
  /**
   * [x] logging
   * [x] parent
   * [x] refresh
   * [x] hwa
   * [x] hwd
   * [x] platform
   * [x] supported enc
   * [x] supported hwacc
   *
   */
  // logging, parent, refresh, hwa, hwd, hwe, platform, supported enc, supported
  // hwacc

  argumentRegistry->add(
      Command::DISPLAYREFRESH,
      std::make_unique<IntegerArgument>("Display refresh rate in ms", "-dr",
                                        "--displayrefresh", 1000));
  argumentRegistry->add(Command::HARDWAREACCEL,
                        std::make_unique<EnumArgument<HWAccelerators>>(
                            "Hardware accelerator to use", "-hwa", "--hwaccel",
                            HWAccelerators::INVALID));
  argumentRegistry->add(
      Command::HARDWAREDECODE,
      std::make_unique<FlagArgument>("Use hardware decoding if available",
                                     "-hwd", "--hardwaredecode", true));
  argumentRegistry->add(
      Command::HARDWAREENCODE,
      std::make_unique<FlagArgument>("Use hardware encoding if available",
                                     "-hwe", "--hardwareencode", false));
  argumentRegistry->add(Command::HELP,
                        std::make_unique<FlagArgument>("Print the help message",
                                                       "-h", "--help", false));
  argumentRegistry->add(
      Command::INFO,
      std::make_unique<FlagArgument>("Print information about the input file",
                                     "-i", "--info", false));
  argumentRegistry->add(Command::LOGGINGOPTIONS,
                        std::make_unique<EnumArgument<LoggingOptions>>(
                            "Logging format to use", "-lf", "--loggingformat",
                            LoggingOptions::DEFAULT));

  argumentRegistry->add(Command::PARENT, std::make_unique<FlagArgument>(
                                             "Run as parent process", "-parent",
                                             "--parent", false));
}

void ProgramOptions::parse(std::vector<std::string> args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ProgramOptions::validate(void) {}

void ProgramOptions::fromJSON(nlohmann::json json) {}

nlohmann::json ProgramOptions::toJSON() {
  nlohmann::json programSettings;

  programSettings["arguments"] = this->argumentRegistry->toJSON();

  programSettings["i_args"] = this->i_args;

  programSettings["supported_encoders"] = nlohmann::json::array();
  programSettings["supported_decoders"] = nlohmann::json::array();
  programSettings["supported_hw_accel"] = nlohmann::json::array();

  programSettings["platform"] =
      EnumToStringFactory::get(this->platform).getName();
  programSettings["gpu_provider"] =
      EnumToStringFactory::get(this->preferredGPUProvider).getName();

  for (auto encoder : this->supportedEncoders) {
    std::string enc_name = EnumToStringFactory::get(encoder).getName();
    programSettings["supported_encoders"].push_back(enc_name);
  }

  /*for (auto decoder : this->supportedDecoders) {
    programSettings["supported_decoders"].push_back(Decoders::getValue(decoder));
  }*/

  for (auto hw_accel : this->supportedHWAccel) {
    std::string hwaccel_name = EnumToStringFactory::get(hw_accel).getName();
    programSettings["supported_hw_accel"].push_back(hwaccel_name);
  }

  return programSettings;
}
