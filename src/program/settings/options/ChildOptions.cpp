#include "../../../utils/logging/LogColor.h"
#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../arguments/EnumArgument.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "../arguments/TimeStringArgument.h"
#include "../arguments/VectorArgument.h"
#include "../enums/Encoders.h"
#include "../enums/HWAccelerators.h"
#include "../enums/LoggingOptions.h"
#include "../enums/Tunes.h"
#include "ChildOptions.h"
#include "nlohmann/json.hpp"

ChildOptions::ChildOptions(void) :
  argumentRegistry(new ArgumentRegistry()),
  runningEncoder(Encoders::INVALID),
  runningHWAccel(HWAccelerators::INVALID) {
}

ChildOptions::~ChildOptions(void) {
  if (this->argumentRegistry != nullptr) {
    // safe ptr
    // delete this->argumentRegistry;
  }
}

void ChildOptions::prepare(void) {
  LOG_DEBUG("Preparing ChildOptions...");

  argumentRegistry->add(Command::AMOUNT, std::make_shared<IntegerArgument>("Amount of files to process", "-a", "--amount", 1));
  argumentRegistry->add(Command::AUDIOCHANNELS, std::make_shared<IntegerArgument>("Number of audio channels to use", "-ac", "--audiochannels", 2));
  argumentRegistry->add(Command::AUDIOCODEC, std::make_shared<VectorArgument<std::string>>("Audio codecs to use per stream mapping", "-acodec", "--audiocodec", std::vector<std::string>()));
  argumentRegistry->add(Command::AUDIOSTREAMS, std::make_shared<VectorArgument<int>>("Index of audio streams to include", "-as", "--audiostreams", std::vector<int>()));
  argumentRegistry->add(Command::BITRATE, std::make_shared<FlagArgument>("Use bitrate instead of CRF", "-b", "--bitrate", false));
  argumentRegistry->add(Command::CROP, std::make_shared<FlagArgument>("Crop the video to the specified ratio", "-c", "--crop", false));
  argumentRegistry->add(Command::CONSTRAIN, std::make_shared<FlagArgument>("Constrain the bitrate to the specified value", "-co", "--constrain", false));
  argumentRegistry->add(Command::CRF, std::make_shared<IntegerArgument>("Constant Rate Factor (CRF) value", "-crf", "--crf", -1));
  argumentRegistry->add(Command::DISPLAYREFRESH, std::make_shared<IntegerArgument>("Display refresh rate in ms", "-dr", "--displayrefresh", 1000));
  argumentRegistry->add(Command::ENCODER, std::make_shared<EnumArgument<Encoders>>("Encoder to use", "-e", "--encoder", Encoders::INVALID));
  argumentRegistry->add(Command::HARDWAREACCEL, std::make_shared<EnumArgument<HWAccelerators>>("Hardware accelerator to use", "-hwa", "--hwaccel", HWAccelerators::INVALID));
  argumentRegistry->add(Command::HARDWAREDECODE, std::make_shared<FlagArgument>("Use hardware decoding if available", "-hwd", "--hardwaredecode", true));
  argumentRegistry->add(Command::HARDWAREENCODE, std::make_shared<FlagArgument>("Use hardware encoding if available", "-hwe", "--hardwareencode", false));
  argumentRegistry->add(Command::HELP, std::make_shared<FlagArgument>("Print the help message", "-h", "--help", false));
  argumentRegistry->add(Command::INFO, std::make_shared<FlagArgument>("Print information about the input file", "-i", "--info", false));
  argumentRegistry->add(Command::LOGGINGOPTIONS, std::make_shared<EnumArgument<LoggingOptions>>("Logging format to use", "-l", "--logging", LoggingOptions::DEFAULT));
  argumentRegistry->add(Command::OVERWRITE, std::make_shared<FlagArgument>("Overwrite existing files", "-o", "--overwrite", false));
  argumentRegistry->add(Command::START, std::make_shared<TimeStringArgument>("Start time for processing", "-ss", "--start", "00:00:00"));
  argumentRegistry->add(Command::TRIM, std::make_shared<TimeStringArgument>("Trim the video to the specified duration", "-tr", "--trim", "00:00:00"));
  argumentRegistry->add(Command::TUNE, std::make_shared<EnumArgument<Tunes>>("Tune to use", "-t", "--tune", Tunes::DEFAULT));
}

void ChildOptions::parse(std::vector<std::string>& args) {

}

void ChildOptions::validate(void) {
  if (this->argumentRegistry->get_t<EnumArgument<Encoders>>(Command::ENCODER)->get() == Encoders::INVALID) {
    this->runningEncoder = Encoders::HEVC;
  }
  else {
    this->runningEncoder = this->argumentRegistry->get_t<EnumArgument<Encoders>>(Command::ENCODER)->get();
  }


  if (!Program::settings->programOptions->supportedHWAccel.empty()) {
    this->runningHWAccel = Program::settings->programOptions->supportedHWAccel[0];
  }

  switch (this->argumentRegistry->get_t<EnumArgument<Encoders>>(Command::ENCODER)->get()) {
  case Encoders::AV1:
  case Encoders::AV1_AMF:
  case Encoders::AV1_NVENC:
  case Encoders::AV1_QSV:
    this->argumentRegistry->get_t<EnumArgument<Tunes>>(Command::TUNE)->set(Tunes::DEFAULT);
    break;
  default:
    break;
  }

  LOG_DEBUG("Running Encoder:", EnumToStringFactory::get(this->runningEncoder).getName());

  // hevc does not support film tune
  // i'm sure av1 does not as well
  if (this->argumentRegistry->get_t<EnumArgument<Tunes>>(Command::TUNE)->get() == Tunes::FILM) {
    switch (this->argumentRegistry->get_t<EnumArgument<Encoders>>(Command::ENCODER)->get()) {
    case Encoders::HEVC:
    case Encoders::HEVC_AMF:
    case Encoders::HEVC_NVENC:
    case Encoders::HEVC_QSV:
      LOG_DEBUG("HEVC does not support film tune.");
      this->argumentRegistry->get_t<EnumArgument<Tunes>>(Command::TUNE)->set(Tunes::DEFAULT);
      break;
    default:
      break;
    }
  }

  LOG_DEBUG("Running Tune:", EnumToStringFactory::get(this->argumentRegistry->get_t<EnumArgument<Tunes>>(Command::TUNE)->get()).getName());
}

void ChildOptions::invalidArgument(std::string arg) {
  LOG(LogColor::fgRed(arg));
  Program::stopFlag = true;
}

void ChildOptions::fromJSON(const nlohmann::json json) {};

nlohmann::json ChildOptions::toJSON() {
  nlohmann::json ChildOptions;

  ChildOptions["argumentRegistry"] = this->argumentRegistry->toJSON();

  ChildOptions["runningEncoder"] = EnumToStringFactory::get(this->runningEncoder).getName();
  ChildOptions["runningHWAccel"] = EnumToStringFactory::get(this->runningHWAccel).getName();

  return ChildOptions;
}
