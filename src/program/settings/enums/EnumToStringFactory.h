#ifndef ENUM_TO_STRING_FACTORY_H
#define ENUM_TO_STRING_FACTORY_H

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
#include "EnumToStringRegistry.h"
#include "GPUProviders.h"
#include "HWAccelerators.h"
#include "LoggingOptions.h"
#include "Platform.h"
#include "SubtitleCodec.h"
#include "Tunes.h"

class EnumToStringFactory {
public:
  static void init(void) {
    registries[typeid(Activity)] = new EnumToStringRegistry<Activity>();
    registries[typeid(Encoders)] = new EnumToStringRegistry<Encoders>();
    registries[typeid(HWAccelerators)] =
        new EnumToStringRegistry<HWAccelerators>();
    registries[typeid(Decoders)] = new EnumToStringRegistry<Decoders>();
    registries[typeid(GPUProviders)] = new EnumToStringRegistry<GPUProviders>();
    registries[typeid(Tunes)] = new EnumToStringRegistry<Tunes>();
    registries[typeid(Platform)] = new EnumToStringRegistry<Platform>();
    registries[typeid(LoggingOptions)] =
        new EnumToStringRegistry<LoggingOptions>();
    registries[typeid(SubtitleCodec)] =
        new EnumToStringRegistry<SubtitleCodec>();
    registries[typeid(AudioCodec)] = new EnumToStringRegistry<AudioCodec>();
    registries[typeid(Container)] = new EnumToStringRegistry<Container>();

    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::INVALID), "invalid", "Invalid",
              "Invalid activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::CONVERT), "convert", "Convert",
              "Convert activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED), "failed", "Failed",
              "Generic failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_CODEC), "failed_codec",
              "Failed Codec", "Codec failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_CONTAINER), "failed_container",
              "Failed Container", "Container failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_CORRUPT), "failed_corrupt",
              "Failed Corrupt", "Corrupt file failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_FILE), "failed_file",
              "Failed File", "File failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_FILE_MISSING),
              "failed_file_missing", "Failed File Missing",
              "File missing failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_FILE_PERMISSIONS),
              "failed_file_permissions", "Failed File Permissions",
              "File permissions failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_FILE_NOT_RECOGNIZED),
              "failed_file_not_recognized", "Failed File Not Recognized",
              "File not recognized failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_HARDWARE), "failed_hardware",
              "Failed Hardware", "Hardware failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_INVALID_AUDIO_CHANNELS),
              "failed_invalid_audio_channels", "Failed Invalid Audio Channels",
              "Invalid audio channels failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_INVALID_AUDIO_STREAMS),
              "failed_invalid_audio_streams", "Failed Invalid Audio Streams",
              "Invalid audio streams failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_INVALID_DURATION_SS),
              "failed_invalid_duration_ss", "Failed Invalid Duration SS",
              "Invalid duration SS failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_INVALID_DURATION_TO),
              "failed_invalid_duration_to", "Failed Invalid Duration TO",
              "Invalid duration TO failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_INVALID_ENCODER),
              "failed_invalid_encoder", "Failed Invalid Encoder",
              "Invalid encoder failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_JSON_PARSE),
              "failed_json_parse", "Failed JSON Parse", "JSON parse failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FAILED_SYSTEM), "failed_system",
              "Failed System", "System failure");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::FINISHED), "finished", "Finished",
              "Finished activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::STATISTICS), "statistics",
              "Statistics", "Statistics activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::WAITING), "waiting", "Waiting",
              "Waiting activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::WAITING_CONVERT), "waiting_convert",
              "Waiting Convert", "Waiting for convert activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::WAITING_STATISTICS),
              "waiting_statistics", "Waiting Statistics",
              "Waiting for statistics activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::WAITING_VALIDATE), "waiting_validate",
              "Waiting Validate", "Waiting for validate activity");
    registries.at(typeid(Activity))
        ->add(static_cast<int>(Activity::VALIDATE), "validate", "Validate",
              "Validate activity");

    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::INVALID), "invalid", "Invalid",
              "Invalid encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::H264), "h264", "H264", "H264 encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::H264_AMF), "h264_amf", "H264 AMF",
              "H264 AMF encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::H264_NVENC), "h264_nvenc",
              "H264 NVEnc", "H264 NVEnc encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::H264_QSV), "h264_qsv", "H264 QSV",
              "H264 QSV encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::HEVC), "hevc", "HEVC", "HEVC encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::HEVC_AMF), "hevc_amf", "HEVC AMF",
              "HEVC AMF encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::HEVC_NVENC), "hevc_nvenc",
              "HEVC NVEnc", "HEVC NVEnc encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::HEVC_QSV), "hevc_qsv", "HEVC QSV",
              "HEVC QSV encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::AV1), "av1", "AV1", "AV1 encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::AV1_AMF), "av1_amf", "AV1 AMF",
              "AV1 AMF encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::AV1_NVENC), "av1_nvenc", "AV1 NVEnc",
              "AV1 NVEnc encoder");
    registries.at(typeid(Encoders))
        ->add(static_cast<int>(Encoders::AV1_QSV), "av1_qsv", "AV1 QSV",
              "AV1 QSV encoder");

    registries.at(typeid(HWAccelerators))
        ->add(static_cast<int>(HWAccelerators::INVALID), "invalid", "Invalid",
              "Invalid hardware accelerator");
    registries.at(typeid(HWAccelerators))
        ->add(static_cast<int>(HWAccelerators::AMD), "amf", "AMF",
              "AMD hardware accelerator");
    registries.at(typeid(HWAccelerators))
        ->add(static_cast<int>(HWAccelerators::NVIDIA), "cuda", "CUDA",
              "NVIDIA hardware accelerator");
    registries.at(typeid(HWAccelerators))
        ->add(static_cast<int>(HWAccelerators::INTEL), "qsv", "QSV",
              "Intel hardware accelerator");
    registries.at(typeid(HWAccelerators))
        ->add(static_cast<int>(HWAccelerators::VULKAN), "vulkan", "Vulkan",
              "Vulkan hardware accelerator");

    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::INVALID), "invalid", "Invalid",
              "Invalid decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::H264_CUVID), "h264_cuvid",
              "H264 CUVID", "H264 CUVID decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::H264_QSV), "h264_qsv", "H264 QSV",
              "H264 QSV decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::HEVC_CUVID), "hevc_cuvid",
              "HEVC CUVID", "HEVC CUVID decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::HEVC_QSV), "hevc_qsv", "HEVC QSV",
              "HEVC QSV decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::AV1_CUVID), "av1_cuvid", "AV1 CUVID",
              "AV1 CUVID decoder");
    registries.at(typeid(Decoders))
        ->add(static_cast<int>(Decoders::AV1_QSV), "av1_qsv", "AV1 QSV",
              "AV1 QSV decoder");

    registries.at(typeid(GPUProviders))
        ->add(static_cast<int>(GPUProviders::INVALID), "invalid", "Invalid",
              "Invalid GPU provider");
    registries.at(typeid(GPUProviders))
        ->add(static_cast<int>(GPUProviders::NVIDIA), "nvidia", "NVIDIA",
              "NVIDIA GPU provider");
    registries.at(typeid(GPUProviders))
        ->add(static_cast<int>(GPUProviders::AMD), "amd", "AMD",
              "AMD GPU provider");
    registries.at(typeid(GPUProviders))
        ->add(static_cast<int>(GPUProviders::INTEL), "intel", "Intel",
              "Intel GPU provider");

    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::INVALID), "invalid", "Invalid",
              "Invalid tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::FAST), "fast", "Fast", "Fast tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::BALANCED), "balanced", "Balanced",
              "Balanced tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::HIGH_QUALITY), "high_quality",
              "High Quality", "High Quality tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::LOW_LATENCY), "low_latency",
              "Low Latency", "Low Latency tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::ZERO_LATENCY), "zero_latency",
              "Zero Latency", "Zero Latency tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::LOSSLESS), "lossless", "Lossless",
              "Lossless tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::PROXY), "proxy", "Proxy", "Proxy tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::ANIMATION), "animation", "Animation",
              "Animation tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::FILM), "film", "Film", "Film tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::GRAIN), "grain", "Grain", "Grain tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::STILL_IMAGE), "still_image",
              "Still Image", "Still Image tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::PSNR), "psnr", "PSNR", "PSNR tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::SSIM), "ssim", "SSIM", "SSIM tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::TEXTURE_COMPRESSION),
              "texture_compression", "Texture Compression",
              "Texture Compression tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::ZERO_LATENCY_LOW_LATENCY),
              "zero_latency_low_latency", "Zero Latency Low Latency",
              "Zero Latency Low Latency tune");
    registries.at(typeid(Tunes))
        ->add(static_cast<int>(Tunes::DEFAULT), "default", "Default",
              "Default tune");

    registries.at(typeid(Platform))
        ->add(static_cast<int>(Platform::INVALID), "invalid", "Invalid",
              "Invalid platform");
    registries.at(typeid(Platform))
        ->add(static_cast<int>(Platform::_LINUX), "linux", "Linux",
              "Linux platform");
    registries.at(typeid(Platform))
        ->add(static_cast<int>(Platform::WINDOWS), "windows", "Windows",
              "Windows platform");
    registries.at(typeid(Platform))
        ->add(static_cast<int>(Platform::MACOS), "macos", "MacOS",
              "MacOS platform");
    registries.at(typeid(Platform))
        ->add(static_cast<int>(Platform::LINUX_LIKE), "linux_like",
              "Linux Like", "Linux Like platform");

    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::INVALID), "invalid", "Invalid",
              "Invalid logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::DEFAULT), "default", "Default",
              "Default logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::DEBUG), "debug", "Debug",
              "Debug logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::VERBOSE), "verbose", "Verbose",
              "Verbose logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::JSON), "json", "JSON",
              "JSON logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::JSON_DEBUG), "json_debug",
              "JSON Debug", "JSON Debug logging format");
    registries.at(typeid(LoggingOptions))
        ->add(static_cast<int>(LoggingOptions::JSON_VERBOSE), "json_verbose",
              "JSON Verbose", "JSON Verbose logging format");

    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::INVALID), "invalid", "Invalid",
              "Invalid subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::SRT), "srt", "SRT",
              "SRT subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::ASS), "ass", "ASS",
              "Advanced Substation Alpha (ASS) Subtitle");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::SRT), "srt", "SRT",
              "SRT subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::ASS), "ass", "ASS",
              "Advanced Substation Alpha (ASS) Subtitle");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::SSA), "ssa", "SSA", "SSA");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::SUBRIP), "subrip", "SUBRIP",
              "SUBRIP subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::DVB_SUB), "dvb-sub", "DVB Sub",
              "DVB subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::WEBVTT), "webvtt", "WebVTT",
              "WebVTT subtitle codec");
    registries.at(typeid(SubtitleCodec))
        ->add(static_cast<int>(SubtitleCodec::MOV_TEXT), "mov", "MOV",
              "QuickTime (MOV)");

    registries.at(typeid(AudioCodec))
        ->add(static_cast<int>(SubtitleCodec::INVALID), "invalid", "Invalid",
              "Invalid audio codec");

    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::INVALID), "invalid", "Invalid",
              "Invalid audio codec");
    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::MKV), "mkv", "MKV",
              "Matroska (MKV) container");
    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::MP4), "mp4", "MP4",
              "MPEG-4 Part 14 (MP4) container");
    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::AVI), "avi", "AVI",
              "Audio Video Interleave (AVI) container");
    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::MOV), "mov", "MOV",
              "QuickTime (MOV) container");
    registries.at(typeid(Container))
        ->add(static_cast<int>(Container::MP3), "mp3", "MP3",
              "MPEG Audio Layer III (MP3) container");
  };

  template <EnumType T> static EnumToStringRegistry<T> *getRegistry() {
    if (!registries.contains(typeid(T))) {
      throw std::logic_error("Registry not initialized for this enum type.");
    }
    return static_cast<EnumToStringRegistry<T> *>(registries.at(typeid(T)));
  }

  template <EnumType T> static const StringEnumDataHolder &get(T key) {
    if (!registries.contains(typeid(T))) {
      throw std::logic_error("Registry not initialized for this enum type.");
    }
    return registries.at(typeid(T))->get(static_cast<int>(key));
  }

  template <EnumType T> static T fromName(std::string &e_value) {
    if (!registries.contains(typeid(T))) {
      throw std::logic_error("Registry not initialized for this enum type.");
    }
    return static_cast<T>(registries.at(typeid(T))->fromName(e_value));
  }

  static void end(void) {
    LOG_DEBUG("Destroying EnumToStringFactory...");
    LOG_DEBUG("Expecting to destroy { EnumToStringRegistry }");
    for (auto &[key, val] : registries) {
      delete val;
    }
  }

private:
  static std::unordered_map<std::type_index, EnumToStringRegistryBase *>
      registries;
};

#endif // !ENUM_TO_STRING_FACTORY_H
