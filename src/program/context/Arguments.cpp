
#include "Arguments.h"
#include <memory>

// acts like prepare
Arguments::Arguments(RuntimeEnvironment &run_env,
                     const std::vector<std::string> &i_args,
                     const std::string &CWD)
    : run_env(run_env), running_encoder(Encoders_N::HEVC),
      running_hw_accel(HWAccelerators_N::NONE), CWD(CWD), i_args(i_args) {

  argumentRegistry
      .add(Command_N::AMOUNT,
           std::make_unique<IntegerArgument>(
               Command_N::definition(Command_N::AMOUNT).getLongDescription(),
               "-a", "--amount", "[int]", 1))
      .add(Command_N::AUDIOCHANNELS,
           std::make_unique<VectorArgument<int>>(
               Command_N::definition(Command_N::AUDIOCHANNELS)
                   .getLongDescription(),
               "-ac", "--audiochannels", "[int,int,...]", std::vector<int>()))
      .add(
          Command_N::AUDIOCODEC,
          std::make_unique<VectorArgument<std::string>>(
              Command_N::definition(Command_N::AUDIOCODEC).getLongDescription(),
              "-aco", "--audiocodec", "[str,str,...]",
              std::vector<std::string>()))
      .add(Command_N::AUDIOSTREAMS,
           std::make_unique<VectorArgument<int>>(
               Command_N::definition(Command_N::AUDIOSTREAMS)
                   .getLongDescription(),
               "-as", "--audiostreams", "[int,int,...]", std::vector<int>()))
      .add(Command_N::AUDIOBITDEPTH,
           std::make_unique<VectorArgument<std::string>>(
               Command_N::definition(Command_N::AUDIOBITDEPTH)
                   .getLongDescription(),
               "-abd", "--audiobitdepth", "[str,str,...]",
               std::vector<std::string>()))
      .add(Command_N::AUDIOSAMPLERATE,
           std::make_unique<VectorArgument<int>>(
               Command_N::definition(Command_N::AUDIOSAMPLERATE)
                   .getLongDescription(),
               "-asr", "--audiosamplerate", "[int,int,...]",
               std::vector<int>()))
      .add(Command_N::AUDIOBITRATE,
           std::make_unique<VectorArgument<int>>(
               Command_N::definition(Command_N::AUDIOBITRATE)
                   .getLongDescription(),
               "-ab", "--audiobitrate", "[int,int,...]", std::vector<int>()))
      .add(Command_N::BITRATE,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::BITRATE).getLongDescription(),
               "-b", "--bitrate", "", false))
      .add(Command_N::CROP,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::CROP).getLongDescription(),
               "-c", "--crop", "", false))
      .add(Command_N::CONSTRAIN,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::CONSTRAIN).getLongDescription(),
               "-co", "--constrain", "", false))
      .add(Command_N::CONTAINER,
           std::make_unique<EnumArgument<Container_N::Container>>(
               Command_N::definition(Command_N::CONTAINER).getLongDescription(),
               "-con", "--container", "[str]", Container_N::MKV))
      .add(Command_N::CRF,
           std::make_unique<IntegerArgument>(
               Command_N::definition(Command_N::CRF).getLongDescription(),
               "-crf", "--crf", "[int]", -1))
      .add(Command_N::DISPLAYREFRESH,
           std::make_unique<IntegerArgument>(
               Command_N::definition(Command_N::DISPLAYREFRESH)
                   .getLongDescription(),
               "-dr", "--displayrefresh", "[int]", 1000))
      .add(Command_N::ENCODER,
           std::make_unique<EnumArgument<Encoders_N::Encoders>>(
               Command_N::definition(Command_N::ENCODER).getLongDescription(),
               "-e", "--encoder", "[str]", Encoders_N::HEVC))
      .add(Command_N::HARDWAREACCEL,
           std::make_unique<EnumArgument<HWAccelerators_N::HWAccelerators>>(
               Command_N::definition(Command_N::HARDWAREACCEL)
                   .getLongDescription(),
               "-hwa", "--hwaccel", "[str]", HWAccelerators_N::INVALID))
      .add(Command_N::HARDWAREDECODE,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::HARDWAREDECODE)
                   .getLongDescription(),
               "-hwd", "--hardwaredecode", "", true))
      .add(Command_N::HARDWAREENCODE,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::HARDWAREENCODE)
                   .getLongDescription(),
               "-hwe", "--hardwareencode", "", false))
      .add(Command_N::HELP,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::HELP).getLongDescription(),
               "-h", "--help", "", false))
      .add(Command_N::INFO,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::INFO).getLongDescription(),
               "-i", "--info", "", false))
      .add(Command_N::OVERWRITE,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::OVERWRITE).getLongDescription(),
               "-o", "--overwrite", "", false))
      .add(Command_N::LOGGINGOPTIONS,
           std::make_unique<EnumArgument<LogFormat_N::LogFormat>>(
               Command_N::definition(Command_N::LOGGINGOPTIONS)
                   .getLongDescription(),
               "-lf", "--loggingformat", "[str]", LogFormat_N::DEFAULT))
      .add(Command_N::PARENT,
           std::make_unique<FlagArgument>(
               Command_N::definition(Command_N::PARENT).getLongDescription(),
               "-parent", "--parent", "", false))
      .add(Command_N::QUALITY,
           std::make_unique<Quality>(
               Command_N::definition(Command_N::QUALITY).getLongDescription(),
               "-q", "--quality", "[str]",
               MediaDefinedFormat::formats.at("720p")))
      .add(Command_N::START,
           std::make_unique<TimeStringArgument>(
               Command_N::definition(Command_N::START).getLongDescription(),
               "-ss", "--start", "[hh:mm:ss]", ""))
      .add(Command_N::SUBTITLECODECS,
           std::make_unique<VectorArgument<std::string>>(
               Command_N::definition(Command_N::SUBTITLECODECS)
                   .getLongDescription(),
               "-sco", "--subtitlecodecs", "[str,str,...]",
               std::vector<std::string>()))
      .add(Command_N::SUBTITLESTREAMS,
           std::make_unique<VectorArgument<int>>(
               Command_N::definition(Command_N::SUBTITLESTREAMS)
                   .getLongDescription(),
               "-sst", "--subtitlestreams", "[int,int,...]",
               std::vector<int>()))
      .add(Command_N::TRIM,
           std::make_unique<TimeStringVectorArgument>(
               Command_N::definition(Command_N::TRIM).getLongDescription(),
               "-tr", "--trim", "[hh:mm:ss,hh:mm:ss]",
               std::vector<std::string>()))
      .add(Command_N::TUNE,
           std::make_unique<EnumArgument<Tunes_N::Tunes>>(
               Command_N::definition(Command_N::TUNE).getLongDescription(),
               "-t", "--tune", "[str]", Tunes_N::DEFAULT));
}

std::shared_ptr<Arguments> Arguments::parse(RuntimeEnvironment &run_env,
                                            std::vector<std::string> &args) {
  std::shared_ptr<Arguments> arguments =
      std::make_shared<Arguments>(run_env, args, args[0]);

  arguments->argumentRegistry.parse(args);

  return arguments;
}

void Arguments::validate() {
  GenericArgument &encoderArg =
      this->argumentRegistry.get<Command_N::ENCODER>();
  GenericArgument &tunesArg = this->argumentRegistry.get<Command_N::TUNE>();

  EnumArgument<Encoders_N::Encoders> *enumArgEncoder =
      dynamic_cast<EnumArgument<Encoders_N::Encoders> *>(&encoderArg);

  EnumArgument<Tunes_N::Tunes> *enumArgTunes =
      dynamic_cast<EnumArgument<Tunes_N::Tunes> *>(&tunesArg);
  if (enumArgEncoder && enumArgEncoder->get() == Encoders_N::INVALID) {
    this->running_encoder = Encoders_N::HEVC;
  } else {
    this->running_encoder = enumArgEncoder->get();
  }

  if (!this->run_env.hw_cap.supportedHWAccel.empty()) {
    this->running_hw_accel = this->run_env.hw_cap.supportedHWAccel[0];
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

  LOG_DEBUG(Logger::Priority::INFO,
            "Running Encoder:", Encoders_N::definition(this->running_encoder));

  LOG_DEBUG(Logger::Priority::INFO, "Running HWAccel:",
            HWAccelerators_N::definition(this->running_hw_accel));

  // hevc does not support film tune
  // i'm sure av1 does not as well
  if (enumArgTunes->get() == Tunes_N::FILM) {
    switch (enumArgEncoder->get()) {
    case Encoders_N::HEVC:
    case Encoders_N::HEVC_AMF:
    case Encoders_N::HEVC_NVENC:
    case Encoders_N::HEVC_QSV:
      LOG_DEBUG(Logger::Priority::INFO, "HEVC does not support film tune.");
      enumArgTunes->set(Tunes_N::DEFAULT);
      break;
    default:
      break;
    }
  }

  LOG_DEBUG(Logger::Priority::INFO, "Running Tune:",
            DefinitionRegistry::defFromEnum(enumArgTunes->get()));
}

void Arguments::fromJSON(const nlohmann::json &json) {
  this->argumentRegistry.fromJSON(json["argumentRegistry"]);
}

nlohmann::json Arguments::toJSON(void) {
  nlohmann::json json;

  json["argumentRegistry"] = this->argumentRegistry.toJSON();

  return json;
}