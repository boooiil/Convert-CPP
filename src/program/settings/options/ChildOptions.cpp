#include "ChildOptions.h"

#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../../child/media/MediaDefinedFormat.h"
#include "../arguments/EnumArgument.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "../arguments/TimeStringArgument.h"
#include "../arguments/TimeStringVectorArgument.h"
#include "../arguments/VectorArgument.h"
#include "../arguments/video/Quality.h"
#include "../enums/Encoders_N.h"
#include "../enums/HWAccelerators_N.h"
#include "../enums/Tunes_N.h"
#include "nlohmann/json.hpp"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/GenericArgument.h"
#include "src/program/settings/enums/Container_N.h"

ChildOptions::ChildOptions(void)
    : runningEncoder(Encoders_N::HEVC), runningHWAccel(HWAccelerators_N::NONE),
      argumentRegistry(new ArgumentRegistry()) {}

ChildOptions::~ChildOptions(void) {
  if (this->argumentRegistry != nullptr) {
    LOG_DEBUG("Destroying ChildOptions...");
    LOG_DEBUG("Expecting to destroy { ArgumentRegistry }");
    if (this->argumentRegistry != nullptr) {
      delete this->argumentRegistry;
    }
  }
}

void ChildOptions::prepare(void) {
  LOG_DEBUG("Preparing ChildOptions...");

  argumentRegistry->add(Command_N::AMOUNT,
                        std::make_unique<IntegerArgument>(
                            "Amount of files to process", "-a", "--amount", 1));
  argumentRegistry->add(Command_N::AUDIOCHANNELS,
                        std::make_unique<VectorArgument<int>>(
                            "Number of audio channels to use", "-ac",
                            "--audiochannels", std::vector<int>()));
  argumentRegistry->add(Command_N::AUDIOCODEC,
                        std::make_unique<VectorArgument<std::string>>(
                            "Audio codecs to use per stream mapping", "-aco",
                            "--audiocodec", std::vector<std::string>()));
  argumentRegistry->add(Command_N::AUDIOSTREAMS,
                        std::make_unique<VectorArgument<int>>(
                            "Index of audio streams to include", "-as",
                            "--audiostreams", std::vector<int>()));
  argumentRegistry->add(Command_N::BITRATE, std::make_unique<FlagArgument>(
                                                "Use bitrate instead of CRF",
                                                "-b", "--bitrate", false));
  argumentRegistry->add(
      Command_N::CROP,
      std::make_unique<FlagArgument>("Crop the video to the specified ratio",
                                     "-c", "--crop", false));
  argumentRegistry->add(Command_N::CONSTRAIN,
                        std::make_unique<FlagArgument>(
                            "Constrain the bitrate to the specified value",
                            "-co", "--constrain", false));
  argumentRegistry->add(
      Command_N::CONTAINER,
      std::make_unique<EnumArgument<Container_N::Container>>(
          "Container to use", "-con", "--container", Container_N::MKV));
  argumentRegistry->add(Command_N::CRF, std::make_unique<IntegerArgument>(
                                            "Constant Rate Factor (CRF) value",
                                            "-crf", "--crf", -1));
  argumentRegistry->add(
      Command_N::ENCODER,
      std::make_unique<EnumArgument<Encoders_N::Encoders>>(
          "Encoder to use", "-e", "--encoder", Encoders_N::HEVC));
  argumentRegistry->add(
      Command_N::HARDWAREACCEL,
      std::make_unique<EnumArgument<HWAccelerators_N::HWAccelerators>>(
          "Hardware accelerator to use", "-hwa", "--hwaccel",
          HWAccelerators_N::INVALID));
  argumentRegistry->add(
      Command_N::HARDWAREDECODE,
      std::make_unique<FlagArgument>("Use hardware decoding if available",
                                     "-hwd", "--hardwaredecode", true));
  argumentRegistry->add(
      Command_N::HARDWAREENCODE,
      std::make_unique<FlagArgument>("Use hardware encoding if available",
                                     "-hwe", "--hardwareencode", false));
  argumentRegistry->add(
      Command_N::INFO,
      std::make_unique<FlagArgument>("Print information about the input file",
                                     "-i", "--info", false));
  argumentRegistry->add(Command_N::OVERWRITE, std::make_unique<FlagArgument>(
                                                  "Overwrite existing files",
                                                  "-o", "--overwrite", false));
  argumentRegistry->add(
      Command_N::QUALITY,
      std::make_unique<Quality>("Quality value", "-q", "--quality",
                                MediaDefinedFormat::formats.at("720p")));
  argumentRegistry->add(Command_N::START,
                        std::make_unique<TimeStringArgument>(
                            "Start time for processing", "-ss", "--start", ""));
  argumentRegistry->add(Command_N::TRIM,
                        std::make_unique<TimeStringVectorArgument>(
                            "Trim the video to the specified duration", "-tr",
                            "--trim", std::vector<std::string>()));
  argumentRegistry->add(Command_N::TUNE,
                        std::make_unique<EnumArgument<Tunes_N::Tunes>>(
                            "Tune to use", "-t", "--tune", Tunes_N::DEFAULT));
}

void ChildOptions::parse(std::vector<std::string> &args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ChildOptions::validate(void) {

  GenericArgument *encoderArg = this->argumentRegistry->get(Command_N::ENCODER);
  GenericArgument *tunesArg = this->argumentRegistry->get(Command_N::TUNE);

  EnumArgument<Encoders_N::Encoders> *enumArgEncoder =
      dynamic_cast<EnumArgument<Encoders_N::Encoders> *>(encoderArg);

  EnumArgument<Tunes_N::Tunes> *enumArgTunes =
      dynamic_cast<EnumArgument<Tunes_N::Tunes> *>(tunesArg);
  if (enumArgEncoder && enumArgEncoder->get() == Encoders_N::INVALID) {
    this->runningEncoder = Encoders_N::HEVC;
  } else {
    this->runningEncoder = enumArgEncoder->get();
  }

  if (!Program::settings->programOptions->supportedHWAccel.empty()) {
    this->runningHWAccel =
        Program::settings->programOptions->supportedHWAccel[0];
  }

  switch (enumArgEncoder->get()) {
  case Encoders_N::AV1:
  case Encoders_N::AV1_AMF:
  case Encoders_N::AV1_NVENC:
  case Encoders_N::AV1_QSV:
    enumArgTunes->set(Tunes_N::DEFAULT);
    break;
  default:
    break;
  }

  LOG_DEBUG("Running Encoder:",
            DefinitionRegistry::defFromEnum(enumArgEncoder->get()));

  LOG_DEBUG("Running HWAccel:",
            DefinitionRegistry::defFromEnum(enumArgEncoder->get()));

  // hevc does not support film tune
  // i'm sure av1 does not as well
  if (enumArgTunes->get() == Tunes_N::FILM) {
    switch (enumArgEncoder->get()) {
    case Encoders_N::HEVC:
    case Encoders_N::HEVC_AMF:
    case Encoders_N::HEVC_NVENC:
    case Encoders_N::HEVC_QSV:
      LOG_DEBUG("HEVC does not support film tune.");
      enumArgTunes->set(Tunes_N::DEFAULT);
      break;
    default:
      break;
    }
  }

  LOG_DEBUG("Running Tune:",
            DefinitionRegistry::defFromEnum(enumArgTunes->get()));
}

void ChildOptions::fromJSON(const nlohmann::json json) { (void)json; };

nlohmann::json ChildOptions::toJSON() {
  nlohmann::json ChildOptions;

  ChildOptions["arguments"] = this->argumentRegistry->toJSON();

  ChildOptions["runningEncoder"] =
      DefinitionRegistry::defFromEnum(this->runningEncoder);
  ChildOptions["runningHWAccel"] =
      DefinitionRegistry::defFromEnum(this->runningHWAccel);

  return ChildOptions;
}
