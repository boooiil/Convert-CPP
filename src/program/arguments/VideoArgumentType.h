#ifndef E4569DBE_35AE_4E25_A12B_58844169254C
#define E4569DBE_35AE_4E25_A12B_58844169254C

#include "src/program/settings/enums/Container_N.h"

struct VideoArgumentType {
  int bitrate = -1;
  int crf = -1;
  int crop_height = -1;
  int crop_width = -1;
  int height = -1;
  int width = -1;

  bool is_bitrate_restricted = false;
  bool is_crf_restricted = true;

  Container_N::Container container = Container_N::Container::MKV;
};

#endif /* E4569DBE_35AE_4E25_A12B_58844169254C */
