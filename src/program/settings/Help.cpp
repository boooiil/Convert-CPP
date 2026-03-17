#include "Help.h"

#include "../../utils/logging/Logger.h"

void Help::printHelp() {
  LOG("Usage: convert [options]\n\n",
       "Options:\n",
       "  -a    --amount [num]                 Set the amount of files to "
       "convert at once. (Default: 1)\n",

       "  -ac,  --audiochannels [num1,num2...] Specify the number of",
       "audio channels for each audio stream.\n",
       "                                       (Will default to original "
       "channel count if not specified)\n",

       "  -af,  --audioformat [str1,str2...]     Specify the audio codec for "
       "each audio stream.\n",
       "                                       (Will default to original codec "
       "if not specified)\n",

       "  -as   --audiostreams [as0,as1...]    Specify which audio streams are "
       "to be used in the final conversion. (Default: ALL)\n",

       "  -b,   --bitrate                      Use the defined bitrate "
       "instead of CRF (Default: False).\n",

       "  -c,   --crop                         Use the defined crop.\n",

       "  -co,  --constrain                    Force the encoder to use a max "
       "bitrate with CRF.\n",

       "  -crf, --crf [num]                    Set a custom CRF value.\n",

       "  -dr,  --displayrefresh [num]         Override the display output "
       "refresh rate in ms. (Default: 1000)\n",

       "  -e,   --encoder [encoder]            Set the encoder to use.\n",

       "  -h,   --help                         Display this help message.\n",

       "  -hwd, --hardwaredecode               Use hardware decoding (Default: "
       "True).\n",

       "  -hwe, --hardwareencode               Use hardware encoding (Default: "
       "False).\n",

       "  -i,   --info                         Display information about the "
       "media or child processes.\n",

       "  -lf,  --loggingformat [str]          Specify the logging format. "
       "(Default: Default)\n",

       "  -o,   --overwrite                    Overwrite existing files "
       "(default: false).\n",

       "  -parent                              Run the program as a parent "
       "process. (Default: False)\n",

       "  -q,   --quality [num]p               Resolution of the media to "
       "convert. (Default: 720p)\n",

       "  -s,   --start [hh:mm:ss]             Skip the beginning by specified "
       "amount of time.\n",

       "  -t,   --tune [tune]                  Set the tune (Default: Film "
       "(x264), None (x265).\n",

       "  -tr,  --trim [hh:mm:ss,hh:mm:ss]     Trim the media between "
       "the given timestamps\n\n",

       "Resolutions:\n",
       "  Custom:\n",
       "    <value>p\n",
       "      CRF: 24\n",
       "      WIDTH: CALCULATED\n",
       "      HEIGHT: PROVIDED\n",
       "      CROP: CALCULATED\n\n",
       "  Configured:\n",
       "    2160p\n",
       "      CRF: 24\n",
       "      Bitrate: 30M\n",
       "      MIN: 30M\n",
       "      MAX: 40M\n",
       "      WIDTH: 3840\n",
       "      HEIGHT: 2160\n",
       "      CROP: 3840:1600 (2.40:1 ~ 21:9)\n"
       "    1440p\n",
       "      CRF: 24\n",
       "      Bitrate: 20M\n",
       "      MIN: 20M\n",
       "      MAX: 27M\n",
       "      WIDTH: 2560\n",
       "      HEIGHT: 1440\n",
       "      CROP: 2560:1066 (2.40:1 ~ 21:9)\n"
       "    1080p\n",
       "      CRF: 24\n",
       "      Bitrate: 2M\n",
       "      MIN: 1.6M\n",
       "      MAX: 2.2M\n",
       "      WIDTH: 1920\n",
       "      HEIGHT: 1080\n",
       "      CROP: 1920:800 (2.40:1 ~ 21:9)\n"
       "    1080pn (Traditional TV/Netflix Cropping)\n",
       "      ...SAME AS 1080p...\n",
       "      CROP: 1920:960 (2:1)\n"
       "    1080pm (Some Marvel Croppings)\n",
       "      ...SAME AS 1080p...\n",
       "      CROP: 1920:870 (64:29)\n"
       "    720p\n",
       "      CRF: 24\n",
       "      Bitrate: 1.4M\n",
       "      MIN: 1.2M\n",
       "      MAX: 1.8M\n",
       "      WIDTH: 1280\n",
       "      HEIGHT: 720\n",
       "      CROP: 1280:534 (2.40:1 ~ 21:9)\n"
       "    720pn (Traditional TV/Netflix Cropping)\n",
       "      ...SAME AS 720p...\n",
       "      CROP: 1280:640 (2:1)\n"
       "    720pm (Some Marvel Croppings)\n",
       "      ...SAME AS 720p...\n",
       "      CROP: 1280:580 (64:29)\n"
       "    480p\n",
       "      CRF: 24\n",
       "      Bitrate: 600K\n",
       "      MIN: 400K\n",
       "      MAX: 800K\n",
       "      WIDTH: 854\n",
       "      HEIGHT: 480\n",
       "      CROP: 854:356 (2.40:1 ~ 21:9)\n"
       "    480pc (NTSC Cropping)\n",
       "      ...SAME AS 480p...\n",
       "      WIDTH: 1138\n",
       "      HEIGHT: 640\n",
       "      CROP: 854:720 (32:27 ~ 4:3)\n"

  );
}
