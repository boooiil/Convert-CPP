#ifndef D0A2DFE9_99D7_4509_B991_E34B696E29AE
#define D0A2DFE9_99D7_4509_B991_E34B696E29AE

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"
#include "src/program/settings/arguments/FlagArgument.h"
#include "src/program/settings/arguments/IntegerArgument.h"
#include "src/program/settings/arguments/TimeStringArgument.h"
#include "src/program/settings/arguments/TimeStringVectorArgument.h"
#include "src/program/settings/arguments/VectorArgument.h"
#include "src/program/settings/arguments/video/Quality.h"
#include "src/program/settings/enums/Container_N.h"
#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/program/settings/enums/LogFormat_N.h"
#include "src/program/settings/enums/Tunes_N.h"

class Command_N {
public:
  enum Command {
    INVALID = 0,

    AMOUNT,          // IntegerArgument
    AUDIOCHANNELS,   // IntegerArgument
    AUDIOCODEC,      // VectorArgument<std::string>
    AUDIOSTREAMS,    // VectorArgument<int>
    AUDIOBITRATE,    // VectorArgument<int>
    BITRATE,         // IntegerArgument
    CROP,            // FlagArgument
    CONSTRAIN,       // FlagArgument
    CONTAINER,       // EnumStringArgument<Container>
    CRF,             // IntegerArgument
    DISPLAYREFRESH,  // IntegerArgument
    ENCODER,         // EnumStringArgument<Encoders>
    HELP,            // FlagArgument
    HARDWAREACCEL,   // EnumStringArgument<HWAccelerators>,
    HARDWAREDECODE,  // FlagArgument
    HARDWAREENCODE,  // FlagArgument
    INFO,            // FlagArgument
    LOGGINGOPTIONS,  // EnumStringArgument<LoggingFormat>,
    OVERWRITE,       // FlagArgument
    PARENT,          // FlagArgument
    QUALITY,         // QualityArgument
    START,           // TimeStringArgument
    SUBTITLECODECS,  // VectorArgument<std::string>
    SUBTITLESTREAMS, // VectorArgument<int>
    TRIM,            // TimeStringArgument
    TUNE,            // EnumStringArgument<Tunes>
  };

  static const Definition &definition(Command command) {

    switch (command) {
    case AMOUNT:
      return DefinitionRegistry::get("command_amount");
    case AUDIOCHANNELS:
      return DefinitionRegistry::get("command_audiochannels");
    case AUDIOCODEC:
      return DefinitionRegistry::get("command_audiocodec");
    case AUDIOSTREAMS:
      return DefinitionRegistry::get("command_audiostreams");
    case AUDIOBITRATE:
      return DefinitionRegistry::get("command_audiobitrate");
    case BITRATE:
      return DefinitionRegistry::get("command_bitrate");
    case CROP:
      return DefinitionRegistry::get("command_crop");
    case CONSTRAIN:
      return DefinitionRegistry::get("command_constrain");
    case CONTAINER:
      return DefinitionRegistry::get("command_container");
    case CRF:
      return DefinitionRegistry::get("command_crf");
    case DISPLAYREFRESH:
      return DefinitionRegistry::get("command_displayrefresh");
    case ENCODER:
      return DefinitionRegistry::get("command_encoder");
    case HELP:
      return DefinitionRegistry::get("command_help");
    case HARDWAREACCEL:
      return DefinitionRegistry::get("command_hardwareaccel");
    case HARDWAREDECODE:
      return DefinitionRegistry::get("command_hardwaredecode");
    case HARDWAREENCODE:
      return DefinitionRegistry::get("command_hardwareencode");
    case INFO:
      return DefinitionRegistry::get("command_info");
    case LOGGINGOPTIONS:
      return DefinitionRegistry::get("command_loggingoptions");
    case OVERWRITE:
      return DefinitionRegistry::get("command_overwrite");
    case PARENT:
      return DefinitionRegistry::get("command_parent");
    case QUALITY:
      return DefinitionRegistry::get("command_quality");
    case START:
      return DefinitionRegistry::get("command_start");
    case TRIM:
      return DefinitionRegistry::get("command_trim");
    case TUNE:
      return DefinitionRegistry::get("command_tune");
    default:
      return DefinitionRegistry::get("command_invalid");
    }
  }

  static const Definition &definition(EnumArgument<Command> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Command> &arg) {
    return definition(arg.get());
  }
};

template <Command_N::Command C> struct CommandValueType;

template <> struct CommandValueType<Command_N::AMOUNT> {
  using type = IntegerArgument;
};

template <> struct CommandValueType<Command_N::AUDIOCHANNELS> {
  using type = VectorArgument<int>;
};

template <> struct CommandValueType<Command_N::AUDIOCODEC> {
  using type = VectorArgument<std::string>;
};

template <> struct CommandValueType<Command_N::AUDIOSTREAMS> {
  using type = VectorArgument<int>;
};

template <> struct CommandValueType<Command_N::BITRATE> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::AUDIOBITRATE> {
  using type = VectorArgument<int>;
};

template <> struct CommandValueType<Command_N::CROP> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::CONSTRAIN> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::CONTAINER> {
  using type = EnumArgument<Container_N::Container>;
};

template <> struct CommandValueType<Command_N::CRF> {
  using type = IntegerArgument;
};

template <> struct CommandValueType<Command_N::DISPLAYREFRESH> {
  using type = IntegerArgument;
};

template <> struct CommandValueType<Command_N::ENCODER> {
  using type = EnumArgument<Encoders_N::Encoders>;
};

template <> struct CommandValueType<Command_N::HELP> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::HARDWAREACCEL> {
  using type = EnumArgument<HWAccelerators_N::HWAccelerators>;
};

template <> struct CommandValueType<Command_N::HARDWAREDECODE> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::HARDWAREENCODE> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::INFO> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::LOGGINGOPTIONS> {
  using type = EnumArgument<LogFormat_N::LogFormat>;
};

template <> struct CommandValueType<Command_N::OVERWRITE> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::PARENT> {
  using type = FlagArgument;
};

template <> struct CommandValueType<Command_N::QUALITY> {
  using type = Quality;
};

template <> struct CommandValueType<Command_N::START> {
  using type = TimeStringArgument;
};

template <> struct CommandValueType<Command_N::SUBTITLECODECS> {
  using type = VectorArgument<std::string>;
};

template <> struct CommandValueType<Command_N::SUBTITLESTREAMS> {
  using type = VectorArgument<int>;
};

template <> struct CommandValueType<Command_N::TRIM> {
  using type = TimeStringVectorArgument;
};

template <> struct CommandValueType<Command_N::TUNE> {
  using type = EnumArgument<Tunes_N::Tunes>;
};

#endif /* D0A2DFE9_99D7_4509_B991_E34B696E29AE */
