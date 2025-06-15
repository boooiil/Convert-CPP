#ifndef SETTINGS_H
#define SETTINGS_H

#include <stduuid/uuid.h>

#include <nlohmann/json_fwd.hpp>

#include "enums/Activity.h"
#include "enums/EnumToStringFactory.h"
#include "enums/EnumToStringRegistry.h"
#include "enums/SubtitleCodec.h"
#include "options/ChildOptions.h"
#include "options/ParentOptions.h"
#include "options/ProgramOptions.h"

class Settings : public JSONSerializable {
 public:
  Settings(void);
  ~Settings(void);

  /*EnumToStringRegistry<Activity>* activityRegistry;
  EnumToStringRegistry<Encoders>* encodersRegistry;
  EnumToStringRegistry<HWAccelerators>* hwAccelRegistry;
  EnumToStringRegistry<Decoders>* decodersRegistry;
  EnumToStringRegistry<GPUProviders>* gpuProvidersRegistry;
  EnumToStringRegistry<Tunes>* tunesRegistry;
  EnumToStringRegistry<Platform>* platformRegistry;
  EnumToStringRegistry<LoggingOptions>* loggingFormatRegistry;
  EnumToStringRegistry<SubtitleCodec>* subtitleCodecRegistry;*/

  // logging, parent, refresh, hwa, hwd, hwe, platform, supported enc, supported
  // hwacc
  ProgramOptions* programOptions;
  // amount
  ParentOptions* parentOptions;
  // amount, ffmpeg
  std::unordered_map<uuids::uuid, ChildOptions*> childOptionsMap;

  std::string cwd;

  void init(std::vector<std::string>& args);

  void fromJSON(nlohmann::json json) override;

  nlohmann::json toJSON(void);
};

#endif  // !SETTINGS_H
