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
#include "../enums/Encoders.h"
#include "../enums/HWAccelerators.h"
#include "../enums/Tunes.h"
#include "nlohmann/json.hpp"
#include "src/program/settings/arguments/GenericArgument.h"

ChildOptions::ChildOptions(void)
    : runningEncoder(Encoders::INVALID),
      runningHWAccel(HWAccelerators::INVALID),
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

  argumentRegistry->add(Command::AMOUNT,
                        std::make_unique<IntegerArgument>(
                            "Amount of files to process", "-a", "--amount", 1));
  argumentRegistry->add(Command::AUDIOCHANNELS,
                        std::make_unique<VectorArgument<int>>(
                            "Number of audio channels to use", "-ac",
                            "--audiochannels", std::vector<int>()));
  argumentRegistry->add(Command::AUDIOCODEC,
                        std::make_unique<VectorArgument<std::string>>(
                            "Audio codecs to use per stream mapping", "-aco",
                            "--audiocodec", std::vector<std::string>()));
  argumentRegistry->add(Command::AUDIOSTREAMS,
                        std::make_unique<VectorArgument<int>>(
                            "Index of audio streams to include", "-as",
                            "--audiostreams", std::vector<int>()));
  argumentRegistry->add(Command::BITRATE, std::make_unique<FlagArgument>(
                                              "Use bitrate instead of CRF",
                                              "-b", "--bitrate", false));
  argumentRegistry->add(
      Command::CROP,
      std::make_unique<FlagArgument>("Crop the video to the specified ratio",
                                     "-c", "--crop", false));
  argumentRegistry->add(Command::CONSTRAIN,
                        std::make_unique<FlagArgument>(
                            "Constrain the bitrate to the specified value",
                            "-co", "--constrain", false));
  argumentRegistry->add(
      Command::CONTAINER,
      std::make_unique<EnumArgument<Container>>("Container to use", "-con",
                                                "--container", Container::MKV));
  argumentRegistry->add(Command::CRF, std::make_unique<IntegerArgument>(
                                          "Constant Rate Factor (CRF) value",
                                          "-crf", "--crf", -1));
  argumentRegistry->add(
      Command::ENCODER,
      std::make_unique<EnumArgument<Encoders>>("Encoder to use", "-e",
                                               "--encoder", Encoders::HEVC));
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
  argumentRegistry->add(
      Command::INFO,
      std::make_unique<FlagArgument>("Print information about the input file",
                                     "-i", "--info", false));
  argumentRegistry->add(Command::OVERWRITE, std::make_unique<FlagArgument>(
                                                "Overwrite existing files",
                                                "-o", "--overwrite", false));
  argumentRegistry->add(
      Command::QUALITY,
      std::make_unique<Quality>("Quality value", "-q", "--quality",
                                MediaDefinedFormat::formats.at("720p")));
  argumentRegistry->add(Command::START,
                        std::make_unique<TimeStringArgument>(
                            "Start time for processing", "-ss", "--start", ""));
  argumentRegistry->add(Command::TRIM,
                        std::make_unique<TimeStringVectorArgument>(
                            "Trim the video to the specified duration", "-tr",
                            "--trim", std::vector<std::string>()));
  argumentRegistry->add(Command::TUNE,
                        std::make_unique<EnumArgument<Tunes>>(
                            "Tune to use", "-t", "--tune", Tunes::DEFAULT));
}

void ChildOptions::parse(std::vector<std::string> &args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ChildOptions::validate(void) {

  GenericArgument *encoderArg = this->argumentRegistry->get(Command::ENCODER);
  GenericArgument *tunesArg = this->argumentRegistry->get(Command::TUNE);

  EnumArgument<Encoders> *enumArgEncoder =
      dynamic_cast<EnumArgument<Encoders> *>(encoderArg);

  EnumArgument<Tunes> *enumArgTunes =
      dynamic_cast<EnumArgument<Tunes> *>(tunesArg);

  if (enumArgEncoder && enumArgEncoder->get() == Encoders::INVALID) {
    this->runningEncoder = Encoders::HEVC;
  } else {
    this->runningEncoder = enumArgEncoder->get();
  }

  if (!Program::settings->programOptions->supportedHWAccel.empty()) {
    this->runningHWAccel =
        Program::settings->programOptions->supportedHWAccel[0];
  }

  switch (enumArgEncoder->get()) {
  case Encoders::AV1:
  case Encoders::AV1_AMF:
  case Encoders::AV1_NVENC:
  case Encoders::AV1_QSV:
    enumArgTunes->set(Tunes::DEFAULT);
    break;
  default:
    break;
  }

  LOG_DEBUG("Running Encoder:",
            EnumToStringFactory::get(this->runningEncoder).getName());

  // hevc does not support film tune
  // i'm sure av1 does not as well
  if (enumArgTunes->get() == Tunes::FILM) {
    switch (enumArgEncoder->get()) {
    case Encoders::HEVC:
    case Encoders::HEVC_AMF:
    case Encoders::HEVC_NVENC:
    case Encoders::HEVC_QSV:
      LOG_DEBUG("HEVC does not support film tune.");
      enumArgTunes->set(Tunes::DEFAULT);
      break;
    default:
      break;
    }
  }

  LOG_DEBUG("Running Tune:",
            EnumToStringFactory::get(enumArgTunes->get()).getName());
}

void ChildOptions::fromJSON(const nlohmann::json json) { (void)json; };

nlohmann::json ChildOptions::toJSON() {
  nlohmann::json ChildOptions;

  ChildOptions["arguments"] = this->argumentRegistry->toJSON();

  ChildOptions["runningEncoder"] =
      EnumToStringFactory::get(this->runningEncoder).getName();
  ChildOptions["runningHWAccel"] =
      EnumToStringFactory::get(this->runningHWAccel).getName();

  return ChildOptions;
}
