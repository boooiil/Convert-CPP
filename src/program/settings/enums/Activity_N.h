#ifndef A38A815A_8529_4134_BBBF_5ED292D15D89
#define A38A815A_8529_4134_BBBF_5ED292D15D89

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"
#include "src/utils/logging/LogColor.h"

class Activity_N {
public:
  enum Activity {
    INVALID = 0,

    CONVERT,
    FAILED,
    FAILED_CODEC,
    FAILED_CONTAINER,
    FAILED_CORRUPT,
    FAILED_FILE,
    FAILED_FILE_MISSING,
    FAILED_FILE_PERMISSIONS,
    FAILED_FILE_NOT_RECOGNIZED,
    FAILED_HARDWARE,
    FAILED_INVALID_AUDIO_CHANNELS,
    FAILED_INVALID_AUDIO_STREAMS,
    FAILED_INVALID_DURATION_SS,
    FAILED_INVALID_DURATION_TO,
    FAILED_INVALID_ENCODER,
    FAILED_JSON_PARSE,
    FAILED_SUBTITLE_ENCODING_INVALID,
    FAILED_SYSTEM,
    FINISHED,
    STATISTICS,
    WAITING,
    WAITING_CONVERT,
    WAITING_STATISTICS,
    WAITING_VALIDATE,
    VALIDATE
  };

  static const Definition &definition(Activity activity) {

    switch (activity) {
    case CONVERT:
      return DefinitionRegistry::get("activity_convert");
    case FAILED:
      return DefinitionRegistry::get("activity_failed");
    case FAILED_CODEC:
      return DefinitionRegistry::get("activity_failed_codec");
    case FAILED_CONTAINER:
      return DefinitionRegistry::get("activity_failed_container");
    case FAILED_CORRUPT:
      return DefinitionRegistry::get("activity_failed_corrupt");
    case FAILED_FILE:
      return DefinitionRegistry::get("activity_failed_file");
    case FAILED_FILE_MISSING:
      return DefinitionRegistry::get("activity_failed_file_missing");
    case FAILED_FILE_PERMISSIONS:
      return DefinitionRegistry::get("activity_failed_file_permissions");
    case FAILED_FILE_NOT_RECOGNIZED:
      return DefinitionRegistry::get("activity_failed_file_not_recognized");
    case FAILED_HARDWARE:
      return DefinitionRegistry::get("activity_failed_hardware");
    case FAILED_INVALID_AUDIO_CHANNELS:
      return DefinitionRegistry::get("activity_failed_invalid_audio_channels");
    case FAILED_INVALID_AUDIO_STREAMS:
      return DefinitionRegistry::get("activity_failed_invalid_audio_streams");
    case FAILED_INVALID_DURATION_SS:
      return DefinitionRegistry::get("activity_failed_invalid_duration_ss");
    case FAILED_INVALID_DURATION_TO:
      return DefinitionRegistry::get("activity_failed_invalid_duration_to");
    case FAILED_INVALID_ENCODER:
      return DefinitionRegistry::get("activity_failed_invalid_encoder");
    case FAILED_JSON_PARSE:
      return DefinitionRegistry::get("activity_failed_json_parse");
    case FAILED_SYSTEM:
      return DefinitionRegistry::get("activity_failed_system");
    case FINISHED:
      return DefinitionRegistry::get("activity_finished");
    case STATISTICS:
      return DefinitionRegistry::get("activity_statistics");
    case WAITING:
      return DefinitionRegistry::get("activity_waiting");
    case WAITING_CONVERT:
      return DefinitionRegistry::get("activity_waiting_convert");
    case WAITING_STATISTICS:
      return DefinitionRegistry::get("activity_waiting_statistics");
    case WAITING_VALIDATE:
      return DefinitionRegistry::get("activity_waiting_validate");
    case VALIDATE:
      return DefinitionRegistry::get("activity_validate");
    default:
      return DefinitionRegistry::get("activity_invalid");
    }
  }

  static const std::string getLetter(Activity activity) {
    switch (activity) {
    case CONVERT:
      return LogColor::fgOrange("C");
    case STATISTICS:
      return LogColor::fgWhite("S");
    case VALIDATE:
      return LogColor::fgYellow("V");
    case FINISHED:
      return LogColor::fgGreen("D");
    case WAITING:
    case WAITING_STATISTICS:
    case WAITING_CONVERT:
    case WAITING_VALIDATE:
      return LogColor::fgGray("W");
    default:
      return LogColor::fgRed("F");
    }
  }

  static const Definition &definition(EnumArgument<Activity> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Activity> &arg) {
    return definition(arg.get());
  }

  static const bool isFailed(Activity activity) {
    return activity >= FAILED && activity <= FAILED_SYSTEM;
  }

  static const bool isProcessing(Activity activity) {
    return activity == CONVERT || activity == STATISTICS ||
           activity == VALIDATE;
  }
};

#endif /* A38A815A_8529_4134_BBBF_5ED292D15D89 */
