#ifndef COMMAND_H
#define COMMAND_H

enum class Command {
  AMOUNT,         // IntegerArgument
  AUDIOCHANNELS,  // IntegerArgument
  AUDIOCODEC,     // VectorArgument<std::string>
  AUDIOSTREAMS,   // VectorArgument<int>
  BITRATE,        // IntegerArgument
  CROP,           // FlagArgument
  CONSTRAIN,      // FlagArgument
  CRF,            // IntegerArgument
  DISPLAYREFRESH, // IntegerArgument
  ENCODER,        // EnumStringArgument<Encoders>
  HELP,           // FlagArgument
  HARDWAREACCEL,  // EnumStringArgument<HWAccelerators>,
  HARDWAREDECODE, // FlagArgument
  HARDWAREENCODE, // FlagArgument
  INFO,           // FlagArgument
  LOGGINGOPTIONS, // EnumStringArgument<LoggingFormat>,
  OVERWRITE,      // FlagArgument
  PARENT,         // FlagArgument
  QUALITY,        // QualityArgument
  START,          // TimeStringArgument
  TRIM,           // TimeStringArgument
  TUNE,           // EnumStringArgument<Tunes>
};

#endif // COMMAND_H

// include enum options in registry explicitly
