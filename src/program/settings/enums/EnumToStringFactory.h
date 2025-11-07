#ifndef ENUM_TO_STRING_FACTORY_H
#define ENUM_TO_STRING_FACTORY_H

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "../../../utils/logging/Logger.h"
#include "Activity.h"
#include "AudioCodec.h"
#include "Container.h"
#include "Decoders.h"
#include "Encoders.h"
#include "GPUProviders.h"
#include "HWAccelerators.h"
#include "LoggingOptions.h"
#include "Platform.h"
#include "SubtitleCodec.h"
#include "Tunes.h"
#include "src/program/registry/EnumToStringRegistry.h"
#include "src/program/registry/EnumToStringRegistryMaster.h"

// TODO: generate this file

class EnumToStringFactory {
public:
  static void init(void) {
    registries
        .add(typeid(Activity),
             std::make_unique<EnumToStringRegistry<Activity>>())
        .add(typeid(Encoders),
             std::make_unique<EnumToStringRegistry<Encoders>>())
        .add(typeid(HWAccelerators),
             std::make_unique<EnumToStringRegistry<HWAccelerators>>())
        .add(typeid(Decoders),
             std::make_unique<EnumToStringRegistry<Decoders>>())
        .add(typeid(GPUProviders),
             std::make_unique<EnumToStringRegistry<GPUProviders>>())
        .add(typeid(Tunes), std::make_unique<EnumToStringRegistry<Tunes>>())
        .add(typeid(Platform),
             std::make_unique<EnumToStringRegistry<Platform>>())
        .add(typeid(LoggingOptions),
             std::make_unique<EnumToStringRegistry<LoggingOptions>>())
        .add(typeid(SubtitleCodec),
             std::make_unique<EnumToStringRegistry<SubtitleCodec>>())
        .add(typeid(AudioCodec),
             std::make_unique<EnumToStringRegistry<AudioCodec>>())
        .add(typeid(Container),
             std::make_unique<EnumToStringRegistry<Container>>());

    auto activityRegistry = registries.get(typeid(Activity));
    auto encoderRegistry = registries.get(typeid(Encoders));
    auto hwaccelRegistry = registries.get(typeid(HWAccelerators));
    auto decoderRegistry = registries.get(typeid(Decoders));
    auto gpuRegistry = registries.get(typeid(GPUProviders));
    auto tunesRegistry = registries.get(typeid(Tunes));
    auto platformRegistry = registries.get(typeid(Platform));
    auto logOptRegistry = registries.get(typeid(LoggingOptions));
    auto subRegistry = registries.get(typeid(SubtitleCodec));
    auto audioRegistry = registries.get(typeid(AudioCodec));
    auto containerRegistry = registries.get(typeid(Container));

    activityRegistry->_add(static_cast<int>(Activity::INVALID), "invalid",
                           "Invalid", "Invalid activity");
    activityRegistry->_add(static_cast<int>(Activity::CONVERT), "convert",
                           "Convert", "Convert activity");
    activityRegistry->_add(static_cast<int>(Activity::FAILED), "failed",
                           "Failed", "Generic failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_CODEC),
                           "failed_codec", "Failed Codec", "Codec failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_CONTAINER),
                           "failed_container", "Failed Container",
                           "Container failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_CORRUPT),
                           "failed_corrupt", "Failed Corrupt",
                           "Corrupt file failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_FILE),
                           "failed_file", "Failed File", "File failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_FILE_MISSING),
                           "failed_file_missing", "Failed File Missing",
                           "File missing failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_FILE_PERMISSIONS),
                           "failed_file_permissions", "Failed File Permissions",
                           "File permissions failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_FILE_NOT_RECOGNIZED),
        "failed_file_not_recognized", "Failed File Not Recognized",
        "File not recognized failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_HARDWARE),
                           "failed_hardware", "Failed Hardware",
                           "Hardware failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_INVALID_AUDIO_CHANNELS),
        "failed_invalid_audio_channels", "Failed Invalid Audio Channels",
        "Invalid audio channels failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_INVALID_AUDIO_STREAMS),
        "failed_invalid_audio_streams", "Failed Invalid Audio Streams",
        "Invalid audio streams failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_INVALID_DURATION_SS),
        "failed_invalid_duration_ss", "Failed Invalid Duration SS",
        "Invalid duration SS failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_INVALID_DURATION_TO),
        "failed_invalid_duration_to", "Failed Invalid Duration TO",
        "Invalid duration TO failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_INVALID_ENCODER),
                           "failed_invalid_encoder", "Failed Invalid Encoder",
                           "Invalid encoder failure");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_JSON_PARSE),
                           "failed_json_parse", "Failed JSON Parse",
                           "JSON parse failure");
    activityRegistry->_add(
        static_cast<int>(Activity::FAILED_SUBTITLE_ENCODING_INVALID),
        "failed_subtitle_encoding_invalid", "Subtitle Encoding Invalid",
        "You can only convert subtitles from text to text or bitmap to "
        "bitmap.");
    activityRegistry->_add(static_cast<int>(Activity::FAILED_SYSTEM),
                           "failed_system", "Failed System", "System failure");
    activityRegistry->_add(static_cast<int>(Activity::FINISHED), "finished",
                           "Finished", "Finished activity");
    activityRegistry->_add(static_cast<int>(Activity::STATISTICS), "statistics",
                           "Statistics", "Statistics activity");
    activityRegistry->_add(static_cast<int>(Activity::WAITING), "waiting",
                           "Waiting", "Waiting activity");
    activityRegistry->_add(static_cast<int>(Activity::WAITING_CONVERT),
                           "waiting_convert", "Waiting Convert",
                           "Waiting for convert activity");
    activityRegistry->_add(static_cast<int>(Activity::WAITING_STATISTICS),
                           "waiting_statistics", "Waiting Statistics",
                           "Waiting for statistics activity");
    activityRegistry->_add(static_cast<int>(Activity::WAITING_VALIDATE),
                           "waiting_validate", "Waiting Validate",
                           "Waiting for validate activity");
    activityRegistry->_add(static_cast<int>(Activity::VALIDATE), "validate",
                           "Validate", "Validate activity");

    // TODO: mark encoder registry
    encoderRegistry->_add(static_cast<int>(Encoders::INVALID), "invalid",
                          "Invalid", "Invalid encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::H264), "h264", "H264",
                          "H264 encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::H264_AMF), "h264_amf",
                          "H264 AMF", "H264 AMF encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::H264_NVENC), "h264_nvenc",
                          "H264 NVEnc", "H264 NVEnc encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::H264_QSV), "h264_qsv",
                          "H264 QSV", "H264 QSV encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::HEVC), "hevc", "HEVC",
                          "HEVC encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::HEVC_AMF), "hevc_amf",
                          "HEVC AMF", "HEVC AMF encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::HEVC_NVENC), "hevc_nvenc",
                          "HEVC NVEnc", "HEVC NVEnc encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::HEVC_QSV), "hevc_qsv",
                          "HEVC QSV", "HEVC QSV encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::AV1), "av1", "AV1",
                          "AV1 encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::AV1_AMF), "av1_amf",
                          "AV1 AMF", "AV1 AMF encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::AV1_NVENC), "av1_nvenc",
                          "AV1 NVEnc", "AV1 NVEnc encoder");
    encoderRegistry->_add(static_cast<int>(Encoders::AV1_QSV), "av1_qsv",
                          "AV1 QSV", "AV1 QSV encoder");

    hwaccelRegistry->_add(static_cast<int>(HWAccelerators::INVALID), "invalid",
                          "Invalid", "Invalid hardware accelerator");
    hwaccelRegistry->_add(static_cast<int>(HWAccelerators::AMD), "amf", "AMF",
                          "AMD hardware accelerator");
    hwaccelRegistry->_add(static_cast<int>(HWAccelerators::NVIDIA), "cuda",
                          "CUDA", "NVIDIA hardware accelerator");
    hwaccelRegistry->_add(static_cast<int>(HWAccelerators::INTEL), "qsv", "QSV",
                          "Intel hardware accelerator");
    hwaccelRegistry->_add(static_cast<int>(HWAccelerators::VULKAN), "vulkan",
                          "Vulkan", "Vulkan hardware accelerator");

    decoderRegistry->_add(static_cast<int>(Decoders::INVALID), "invalid",
                          "Invalid", "Invalid decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::H264_CUVID), "h264_cuvid",
                          "H264 CUVID", "H264 CUVID decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::H264_QSV), "h264_qsv",
                          "H264 QSV", "H264 QSV decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::HEVC_CUVID), "hevc_cuvid",
                          "HEVC CUVID", "HEVC CUVID decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::HEVC_QSV), "hevc_qsv",
                          "HEVC QSV", "HEVC QSV decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::AV1_CUVID), "av1_cuvid",
                          "AV1 CUVID", "AV1 CUVID decoder");
    decoderRegistry->_add(static_cast<int>(Decoders::AV1_QSV), "av1_qsv",
                          "AV1 QSV", "AV1 QSV decoder");

    gpuRegistry->_add(static_cast<int>(GPUProviders::INVALID), "invalid",
                      "Invalid", "Invalid GPU provider");
    gpuRegistry->_add(static_cast<int>(GPUProviders::NVIDIA), "nvidia",
                      "NVIDIA", "NVIDIA GPU provider");
    gpuRegistry->_add(static_cast<int>(GPUProviders::AMD), "amd", "AMD",
                      "AMD GPU provider");
    gpuRegistry->_add(static_cast<int>(GPUProviders::INTEL), "intel", "Intel",
                      "Intel GPU provider");

    tunesRegistry->_add(static_cast<int>(Tunes::INVALID), "invalid", "Invalid",
                        "Invalid tune");
    tunesRegistry->_add(static_cast<int>(Tunes::FAST), "fast", "Fast",
                        "Fast tune");
    tunesRegistry->_add(static_cast<int>(Tunes::BALANCED), "balanced",
                        "Balanced", "Balanced tune");
    tunesRegistry->_add(static_cast<int>(Tunes::HIGH_QUALITY), "high_quality",
                        "High Quality", "High Quality tune");
    tunesRegistry->_add(static_cast<int>(Tunes::LOW_LATENCY), "low_latency",
                        "Low Latency", "Low Latency tune");
    tunesRegistry->_add(static_cast<int>(Tunes::ZERO_LATENCY), "zero_latency",
                        "Zero Latency", "Zero Latency tune");
    tunesRegistry->_add(static_cast<int>(Tunes::LOSSLESS), "lossless",
                        "Lossless", "Lossless tune");
    tunesRegistry->_add(static_cast<int>(Tunes::PROXY), "proxy", "Proxy",
                        "Proxy tune");
    tunesRegistry->_add(static_cast<int>(Tunes::ANIMATION), "animation",
                        "Animation", "Animation tune");
    tunesRegistry->_add(static_cast<int>(Tunes::FILM), "film", "Film",
                        "Film tune");
    tunesRegistry->_add(static_cast<int>(Tunes::GRAIN), "grain", "Grain",
                        "Grain tune");
    tunesRegistry->_add(static_cast<int>(Tunes::STILL_IMAGE), "still_image",
                        "Still Image", "Still Image tune");
    tunesRegistry->_add(static_cast<int>(Tunes::PSNR), "psnr", "PSNR",
                        "PSNR tune");
    tunesRegistry->_add(static_cast<int>(Tunes::SSIM), "ssim", "SSIM",
                        "SSIM tune");
    tunesRegistry->_add(static_cast<int>(Tunes::TEXTURE_COMPRESSION),
                        "texture_compression", "Texture Compression",
                        "Texture Compression tune");
    tunesRegistry->_add(static_cast<int>(Tunes::ZERO_LATENCY_LOW_LATENCY),
                        "zero_latency_low_latency", "Zero Latency Low Latency",
                        "Zero Latency Low Latency tune");
    tunesRegistry->_add(static_cast<int>(Tunes::DEFAULT), "default", "Default",
                        "Default tune");

    platformRegistry->_add(static_cast<int>(Platform::INVALID), "invalid",
                           "Invalid", "Invalid platform");
    platformRegistry->_add(static_cast<int>(Platform::_LINUX), "linux", "Linux",
                           "Linux platform");
    platformRegistry->_add(static_cast<int>(Platform::WINDOWS), "windows",
                           "Windows", "Windows platform");
    platformRegistry->_add(static_cast<int>(Platform::MACOS), "macos", "MacOS",
                           "MacOS platform");
    platformRegistry->_add(static_cast<int>(Platform::LINUX_LIKE), "linux_like",
                           "Linux Like", "Linux Like platform");

    logOptRegistry->_add(static_cast<int>(LoggingOptions::INVALID), "invalid",
                         "Invalid", "Invalid logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::DEFAULT), "default",
                         "Default", "Default logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::DEBUG), "debug",
                         "Debug", "Debug logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::VERBOSE), "verbose",
                         "Verbose", "Verbose logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::JSON), "json", "JSON",
                         "JSON logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::JSON_DEBUG),
                         "json_debug", "JSON Debug",
                         "JSON Debug logging format");
    logOptRegistry->_add(static_cast<int>(LoggingOptions::JSON_VERBOSE),
                         "json_verbose", "JSON Verbose",
                         "JSON Verbose logging format");

    subRegistry->_add(static_cast<int>(SubtitleCodec::INVALID), "invalid",
                      "Invalid", "Invalid subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::SRT), "srt", "SRT",
                      "SRT subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::ASS), "ass", "ASS",
                      "Advanced Substation Alpha (ASS) Subtitle");
    subRegistry->_add(static_cast<int>(SubtitleCodec::SRT), "srt", "SRT",
                      "SRT subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::ASS), "ass", "ASS",
                      "Advanced Substation Alpha (ASS) Subtitle");
    subRegistry->_add(static_cast<int>(SubtitleCodec::SSA), "ssa", "SSA",
                      "SSA");
    subRegistry->_add(static_cast<int>(SubtitleCodec::SUBRIP), "subrip",
                      "SUBRIP", "SUBRIP subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::DVB_SUB), "dvb-sub",
                      "DVB Sub", "DVB subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::WEBVTT), "webvtt",
                      "WebVTT", "WebVTT subtitle codec");
    subRegistry->_add(static_cast<int>(SubtitleCodec::MOV_TEXT), "mov", "MOV",
                      "QuickTime (MOV)");

    audioRegistry->_add(static_cast<int>(AudioCodec::INVALID), "invalid",
                        "Invalid", "Invalid audio codec");

    containerRegistry->_add(static_cast<int>(Container::INVALID), "invalid",
                            "Invalid", "Invalid audio codec");
    containerRegistry->_add(static_cast<int>(Container::MKV), "mkv", "MKV",
                            "Matroska (MKV) container");
    containerRegistry->_add(static_cast<int>(Container::MP4), "mp4", "MP4",
                            "MPEG-4 Part 14 (MP4) container");
    containerRegistry->_add(static_cast<int>(Container::AVI), "avi", "AVI",
                            "Audio Video Interleave (AVI) container");
    containerRegistry->_add(static_cast<int>(Container::MOV), "mov", "MOV",
                            "QuickTime (MOV) container");
    containerRegistry->_add(static_cast<int>(Container::MP3), "mp3", "MP3",
                            "MPEG Audio Layer III (MP3) container");
  };

  template <EnumType T> static T fromName(std::string &e_value) {
    if (!registries.has(typeid(T))) {
      throw std::logic_error("Registry not initialized for this enum type.");
    }
    return static_cast<T>(registries.get(typeid(T))->fromName(e_value));
  }

  static void end(void) {
    return;
    // LOG_DEBUG("Destroying EnumToStringFactory...");
    // LOG_DEBUG("Expecting to destroy { EnumToStringRegistry }");
    // for (auto &[key, val] : registries) {
    //   delete val;
    // }
  }

  template <EnumType T> static const StringEnumDataHolder &get(T key) {
    if (!registries.has(typeid(T))) {
      throw std::logic_error("Registry not initialized for this enum type.");
    }

    return *registries.get(typeid(T))->_get(static_cast<int>(key));
  }

private:
  static EnumToStringRegistryMaster registries;
};

#endif // !ENUM_TO_STRING_FACTORY_H
