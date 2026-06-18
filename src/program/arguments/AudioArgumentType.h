#ifndef E17F85A2_119C_420C_AA65_24D2404BBDB2
#define E17F85A2_119C_420C_AA65_24D2404BBDB2

#include "src/program/settings/enums/AudioCodec_N.h"

struct AudioArgumentType {
  int channels = -1;
  int bit_depth = -1;
  int bitrate = -1;
  int sample_rate = -1;
  int stream_index = -1;

  AudioCodec_N::AudioCodec codec = AudioCodec_N::AudioCodec::INVALID;
};

#endif /* E17F85A2_119C_420C_AA65_24D2404BBDB2 */
