#ifndef D0A2DFE9_99D7_4509_B991_E34B696E29AE
#define D0A2DFE9_99D7_4509_B991_E34B696E29AE

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class Command_N {
public:
  enum Command {
    INVALID = 0,

    AMOUNT,          // IntegerArgument
    AUDIOCHANNELS,   // IntegerArgument
    AUDIOCODEC,      // VectorArgument<std::string>
    AUDIOSTREAMS,    // VectorArgument<int>
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

#endif /* D0A2DFE9_99D7_4509_B991_E34B696E29AE */
