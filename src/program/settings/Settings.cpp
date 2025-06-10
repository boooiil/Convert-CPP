#include "Settings.h"

#include <nlohmann/json.hpp>
#include <string>

#include "../../utils/logging/Logger.h"
#include "enums/Tunes.h"
#include <stduuid/uuid.h>
#include <vector>

Settings::Settings() :
  /*activityRegistry(new EnumToStringRegistry<Activity>()),
  encodersRegistry(new EnumToStringRegistry<Encoders>()),
  hwAccelRegistry(new EnumToStringRegistry<HWAccelerators>()),
  decodersRegistry(new EnumToStringRegistry<Decoders>()),
  gpuProvidersRegistry(new EnumToStringRegistry<GPUProviders>()),
  tunesRegistry(new EnumToStringRegistry<Tunes>()),
  platformRegistry(new EnumToStringRegistry<Platform>()),
  loggingFormatRegistry(new EnumToStringRegistry<LoggingOptions>()),*/
  childOptionsMap({}),
  parentOptions(new ParentOptions()),
  programOptions(new ProgramOptions()) {
}

Settings::~Settings() {
  LOG_DEBUG("deleting { encodersRegistry, hwAccelRegistry, decodersRegistry, gpuProvidersRegistry, tunesRegistry, platformRegistry, qualityRegistry, loggingFormatRegistry, parentOptions, programOptions, childOptions }");
  if (!this->childOptionsMap.empty()) {
    LOG_DEBUG("Deleting child options");
    for (auto& [uuid, childOptions] : this->childOptionsMap) {
      LOG_DEBUG("Deleting child options with uuid:", uuid);
      delete childOptions;
    }
  }
  if (this->parentOptions != nullptr) {
    LOG_DEBUG("Deleting parent options");
    delete this->parentOptions;
  }
  if (this->programOptions != nullptr) {
    LOG_DEBUG("Deleting program options");
    delete this->programOptions;
  }
}

void Settings::init(std::vector<std::string>& args) {
  // LOG_DEBUG("Creating settings object with (",
  //             std::to_string(argc), ") args.");

  // LOG_DEBUG("Creating argument parser object.");

  //this->activityRegistry->add(Activity::INVALID, "invalid", "Invalid", "Invalid activity");
  //this->activityRegistry->add(Activity::CONVERT, "convert", "Convert", "Convert activity");
  //this->activityRegistry->add(Activity::FAILED, "failed", "Failed", "Generic failure");
  //this->activityRegistry->add(Activity::FAILED_CODEC, "failed_codec", "Failed Codec", "Codec failure");
  //this->activityRegistry->add(Activity::FAILED_CONTAINER, "failed_container", "Failed Container", "Container failure");
  //this->activityRegistry->add(Activity::FAILED_CORRUPT, "failed_corrupt", "Failed Corrupt", "Corrupt file failure");
  //this->activityRegistry->add(Activity::FAILED_FILE, "failed_file", "Failed File", "File failure");
  //this->activityRegistry->add(Activity::FAILED_FILE_MISSING, "failed_file_missing", "Failed File Missing", "File missing failure");
  //this->activityRegistry->add(Activity::FAILED_FILE_PERMISSIONS, "failed_file_permissions", "Failed File Permissions", "File permissions failure");
  //this->activityRegistry->add(Activity::FAILED_FILE_NOT_RECOGNIZED, "failed_file_not_recognized", "Failed File Not Recognized", "File not recognized failure");
  //this->activityRegistry->add(Activity::FAILED_HARDWARE, "failed_hardware", "Failed Hardware", "Hardware failure");
  //this->activityRegistry->add(Activity::FAILED_INVALID_AUDIO_CHANNELS, "failed_invalid_audio_channels", "Failed Invalid Audio Channels", "Invalid audio channels failure");
  //this->activityRegistry->add(Activity::FAILED_INVALID_AUDIO_STREAMS, "failed_invalid_audio_streams", "Failed Invalid Audio Streams", "Invalid audio streams failure");
  //this->activityRegistry->add(Activity::FAILED_INVALID_DURATION_SS, "failed_invalid_duration_ss", "Failed Invalid Duration SS", "Invalid duration SS failure");
  //this->activityRegistry->add(Activity::FAILED_INVALID_DURATION_TO, "failed_invalid_duration_to", "Failed Invalid Duration TO", "Invalid duration TO failure");
  //this->activityRegistry->add(Activity::FAILED_INVALID_ENCODER, "failed_invalid_encoder", "Failed Invalid Encoder", "Invalid encoder failure");
  //this->activityRegistry->add(Activity::FAILED_JSON_PARSE, "failed_json_parse", "Failed JSON Parse", "JSON parse failure");
  //this->activityRegistry->add(Activity::FAILED_SYSTEM, "failed_system", "Failed System", "System failure");
  //this->activityRegistry->add(Activity::FINISHED, "finished", "Finished", "Finished activity");
  //this->activityRegistry->add(Activity::STATISTICS, "statistics", "Statistics", "Statistics activity");
  //this->activityRegistry->add(Activity::WAITING, "waiting", "Waiting", "Waiting activity");
  //this->activityRegistry->add(Activity::WAITING_CONVERT, "waiting_convert", "Waiting Convert", "Waiting for convert activity");
  //this->activityRegistry->add(Activity::WAITING_STATISTICS, "waiting_statistics", "Waiting Statistics", "Waiting for statistics activity");
  //this->activityRegistry->add(Activity::WAITING_VALIDATE, "waiting_validate", "Waiting Validate", "Waiting for validate activity");
  //this->activityRegistry->add(Activity::VALIDATE, "validate", "Validate", "Validate activity");

  //this->encodersRegistry->add(Encoders::INVALID, "invalid", "Invalid", "Invalid encoder");
  //this->encodersRegistry->add(Encoders::H264, "h264", "H264", "H264 encoder");
  //this->encodersRegistry->add(Encoders::H264_AMF, "h264_amf", "H264 AMF", "H264 AMF encoder");
  //this->encodersRegistry->add(Encoders::H264_NVENC, "h264_nvenc", "H264 NVEnc", "H264 NVEnc encoder");
  //this->encodersRegistry->add(Encoders::H264_QSV, "h264_qsv", "H264 QSV", "H264 QSV encoder");
  //this->encodersRegistry->add(Encoders::HEVC, "hevc", "HEVC", "HEVC encoder");
  //this->encodersRegistry->add(Encoders::HEVC_AMF, "hevc_amf", "HEVC AMF", "HEVC AMF encoder");
  //this->encodersRegistry->add(Encoders::HEVC_NVENC, "hevc_nvenc", "HEVC NVEnc", "HEVC NVEnc encoder");
  //this->encodersRegistry->add(Encoders::HEVC_QSV, "hevc_qsv", "HEVC QSV", "HEVC QSV encoder");
  //this->encodersRegistry->add(Encoders::AV1, "av1", "AV1", "AV1 encoder");
  //this->encodersRegistry->add(Encoders::AV1_AMF, "av1_amf", "AV1 AMF", "AV1 AMF encoder");
  //this->encodersRegistry->add(Encoders::AV1_NVENC, "av1_nvenc", "AV1 NVEnc", "AV1 NVEnc encoder");
  //this->encodersRegistry->add(Encoders::AV1_QSV, "av1_qsv", "AV1 QSV", "AV1 QSV encoder");

  //this->hwAccelRegistry->add(HWAccelerators::INVALID, "invalid", "Invalid", "Invalid hardware accelerator");
  //this->hwAccelRegistry->add(HWAccelerators::AMD, "amd", "AMD", "AMD hardware accelerator");
  //this->hwAccelRegistry->add(HWAccelerators::NVIDIA, "nvidia", "NVIDIA", "NVIDIA hardware accelerator");
  //this->hwAccelRegistry->add(HWAccelerators::INTEL, "intel", "Intel", "Intel hardware accelerator");
  //this->hwAccelRegistry->add(HWAccelerators::VULKAN, "vulkan", "Vulkan", "Vulkan hardware accelerator");

  //this->decodersRegistry->add(Decoders::INVALID, "invalid", "Invalid", "Invalid decoder");
  //this->decodersRegistry->add(Decoders::H264_CUVID, "h264_cuvid", "H264 CUVID", "H264 CUVID decoder");
  //this->decodersRegistry->add(Decoders::H264_QSV, "h264_qsv", "H264 QSV", "H264 QSV decoder");
  //this->decodersRegistry->add(Decoders::HEVC_CUVID, "hevc_cuvid", "HEVC CUVID", "HEVC CUVID decoder");
  //this->decodersRegistry->add(Decoders::HEVC_QSV, "hevc_qsv", "HEVC QSV", "HEVC QSV decoder");
  //this->decodersRegistry->add(Decoders::AV1_CUVID, "av1_cuvid", "AV1 CUVID", "AV1 CUVID decoder");
  //this->decodersRegistry->add(Decoders::AV1_QSV, "av1_qsv", "AV1 QSV", "AV1 QSV decoder");

  //this->gpuProvidersRegistry->add(GPUProviders::INVALID, "invalid", "Invalid", "Invalid GPU provider");
  //this->gpuProvidersRegistry->add(GPUProviders::NVIDIA, "nvidia", "NVIDIA", "NVIDIA GPU provider");
  //this->gpuProvidersRegistry->add(GPUProviders::AMD, "amd", "AMD", "AMD GPU provider");
  //this->gpuProvidersRegistry->add(GPUProviders::INTEL, "intel", "Intel", "Intel GPU provider");

  //this->tunesRegistry->add(Tunes::INVALID, "invalid", "Invalid", "Invalid tune");
  //this->tunesRegistry->add(Tunes::FAST, "fast", "Fast", "Fast tune");
  //this->tunesRegistry->add(Tunes::BALANCED, "balanced", "Balanced", "Balanced tune");
  //this->tunesRegistry->add(Tunes::HIGH_QUALITY, "high_quality", "High Quality", "High Quality tune");
  //this->tunesRegistry->add(Tunes::LOW_LATENCY, "low_latency", "Low Latency", "Low Latency tune");
  //this->tunesRegistry->add(Tunes::ZERO_LATENCY, "zero_latency", "Zero Latency", "Zero Latency tune");
  //this->tunesRegistry->add(Tunes::LOSSLESS, "lossless", "Lossless", "Lossless tune");
  //this->tunesRegistry->add(Tunes::PROXY, "proxy", "Proxy", "Proxy tune");
  //this->tunesRegistry->add(Tunes::ANIMATION, "animation", "Animation", "Animation tune");
  //this->tunesRegistry->add(Tunes::FILM, "film", "Film", "Film tune");
  //this->tunesRegistry->add(Tunes::GRAIN, "grain", "Grain", "Grain tune");
  //this->tunesRegistry->add(Tunes::STILL_IMAGE, "still_image", "Still Image", "Still Image tune");
  //this->tunesRegistry->add(Tunes::PSNR, "psnr", "PSNR", "PSNR tune");
  //this->tunesRegistry->add(Tunes::SSIM, "ssim", "SSIM", "SSIM tune");
  //this->tunesRegistry->add(Tunes::TEXTURE_COMPRESSION, "texture_compression", "Texture Compression", "Texture Compression tune");
  //this->tunesRegistry->add(Tunes::ZERO_LATENCY_LOW_LATENCY, "zero_latency_low_latency", "Zero Latency Low Latency", "Zero Latency Low Latency tune");
  //this->tunesRegistry->add(Tunes::DEFAULT, "default", "Default", "Default tune");

  //this->platformRegistry->add(Platform::INVALID, "invalid", "Invalid", "Invalid platform");
  //this->platformRegistry->add(Platform::LINUX, "linux", "Linux", "Linux platform");
  //this->platformRegistry->add(Platform::WINDOWS, "windows", "Windows", "Windows platform");
  //this->platformRegistry->add(Platform::MACOS, "macos", "MacOS", "MacOS platform");
  //this->platformRegistry->add(Platform::LINUX_LIKE, "linux_like", "Linux Like", "Linux Like platform");

  //this->loggingFormatRegistry->add(LoggingOptions::INVALID, "invalid", "Invalid", "Invalid logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::DEFAULT, "default", "Default", "Default logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::DEBUG, "debug", "Debug", "Debug logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::VERBOSE, "verbose", "Verbose", "Verbose logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::JSON, "json", "JSON", "JSON logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::JSON_DEBUG, "json_debug", "JSON Debug", "JSON Debug logging format");
  //this->loggingFormatRegistry->add(LoggingOptions::JSON_VERBOSE, "json_verbose", "JSON Verbose", "JSON Verbose logging format");

  //this->subtitleCodecRegistry->add(SubtitleCodec::INVALID, "invalid", "Invalid", "Invalid subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::SRT, "srt", "SRT", "SRT subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::ASS, "ass", "ASS", "Advanced Substation Alpha (ASS) Subtitle");

  //this->subtitleCodecRegistry->add(SubtitleCodec::SRT, "srt", "SRT", "SRT subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::ASS, "ass", "ASS", "Advanced Substation Alpha (ASS) Subtitle");
  //this->subtitleCodecRegistry->add(SubtitleCodec::SSA, "ssa", "SSA", "SSA");
  //this->subtitleCodecRegistry->add(SubtitleCodec::SUBRIP, "subrip", "SUBRIP", "SUBRIP subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::DVB_SUB, "dvb-sub", "DVB Sub", "DVB subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::WEBVTT, "webvtt", "WebVTT", "WebVTT subtitle codec");
  //this->subtitleCodecRegistry->add(SubtitleCodec::MOV_TEXT, "mov", "QuickTime (MOV)");

  LOG_DEBUG("Init settings in CWD:", args[0]);
  this->cwd = args[0];

  LOG_DEBUG("Creating program options object.");
  this->programOptions->prepare();
  this->programOptions->parse(args);
  this->programOptions->gatherSystemDetails();
  this->programOptions->validate();
}

void Settings::fromJSON(nlohmann::json json) {}

nlohmann::json Settings::toJSON() {
  using namespace nlohmann;

  // todo: include registries

  json settings;

  settings["ProgramOptions"] = this->programOptions->toJSON();
  settings["ParentOptions"] = this->parentOptions->toJSON();

  for (const auto& [uuid, childOptions] : this->childOptionsMap) {
    settings["ChildOptions"][uuids::to_string(uuid)] = childOptions->toJSON();
  }

  return settings;
}
