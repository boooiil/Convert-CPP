#include "Help.h"

#include "../../utils/logging/Logger.h"
#include "enums/Command_N.h"
#include "src/program/context/Arguments.h"
#include "src/program/registry/Registry.h"
#include "src/program/settings/arguments/GenericArgument.h"
#include <functional>

void Help::printHelp(Arguments &arguments) {

  const Registry<Command_N::Command, GenericArgument> &registry =
      arguments.argumentRegistry;

  std::cout << "Usage: convert [options]\n\n  Options:\n";

  for (std::reference_wrapper<const GenericArgument> &arg : registry.values()) {
    std::cout << std::left << "    " << std::setw(10) << arg.get().getFlag()
              << std::setw(18) << arg.get().getLongFlag() << std::setw(22)
              << arg.get().getTypeDescriptor() << arg.get().getHelpMessage()
              << "\n";
  }

  LOG("Resolutions:\n", "  Custom:\n", "    <value>p\n", "      CRF: 24\n",
      "      WIDTH: CALCULATED\n", "      HEIGHT: PROVIDED\n",
      "      CROP: CALCULATED\n\n", "  Configured:\n", "    2160p\n",
      "      CRF: 24\n", "      Bitrate: 30M\n", "      MIN: 30M\n",
      "      MAX: 40M\n", "      WIDTH: 3840\n", "      HEIGHT: 2160\n",
      "      CROP: 3840:1600 (2.40:1 ~ 21:9)\n"
      "    1440p\n",
      "      CRF: 24\n", "      Bitrate: 20M\n", "      MIN: 20M\n",
      "      MAX: 27M\n", "      WIDTH: 2560\n", "      HEIGHT: 1440\n",
      "      CROP: 2560:1066 (2.40:1 ~ 21:9)\n"
      "    1080p\n",
      "      CRF: 24\n", "      Bitrate: 2M\n", "      MIN: 1.6M\n",
      "      MAX: 2.2M\n", "      WIDTH: 1920\n", "      HEIGHT: 1080\n",
      "      CROP: 1920:800 (2.40:1 ~ 21:9)\n"
      "    1080pn (Traditional TV/Netflix Cropping)\n",
      "      ...SAME AS 1080p...\n",
      "      CROP: 1920:960 (2:1)\n"
      "    1080pm (Some Marvel Croppings)\n",
      "      ...SAME AS 1080p...\n",
      "      CROP: 1920:870 (64:29)\n"
      "    720p\n",
      "      CRF: 24\n", "      Bitrate: 1.4M\n", "      MIN: 1.2M\n",
      "      MAX: 1.8M\n", "      WIDTH: 1280\n", "      HEIGHT: 720\n",
      "      CROP: 1280:534 (2.40:1 ~ 21:9)\n"
      "    720pn (Traditional TV/Netflix Cropping)\n",
      "      ...SAME AS 720p...\n",
      "      CROP: 1280:640 (2:1)\n"
      "    720pm (Some Marvel Croppings)\n",
      "      ...SAME AS 720p...\n",
      "      CROP: 1280:580 (64:29)\n"
      "    480p\n",
      "      CRF: 24\n", "      Bitrate: 600K\n", "      MIN: 400K\n",
      "      MAX: 800K\n", "      WIDTH: 854\n", "      HEIGHT: 480\n",
      "      CROP: 854:356 (2.40:1 ~ 21:9)\n"
      "    480pc (NTSC Cropping)\n",
      "      ...SAME AS 480p...\n", "      WIDTH: 1138\n",
      "      HEIGHT: 640\n", "      CROP: 854:720 (32:27 ~ 4:3)\n"

  );
}
