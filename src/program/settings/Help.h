#ifndef HELP_H
#define HELP_H

#include "src/program/context/Arguments.h"
#include "src/utils/ListUtils.h"
#include "src/utils/NumberUtils.h"

// NOLINTBEGIN(modernize-return-braced-init-list)
inline auto tab(int spaces) -> std::string {
  return std::string(spaces * 2, ' ');
}
// NOLINTEND(modernize-return-braced-init-list)

class Help {
public:
  static void print(Arguments &arguments) {

    const Registry<Command_N::Command, GenericArgument> &registry =
        arguments.argumentRegistry;

    std::cout << "Usage: convert [options]\n\n  Options:\n";

    for (std::reference_wrapper<const GenericArgument> &arg :
         registry.values()) {
      std::cout << std::left << "    " << std::setw(10) << arg.get().getFlag()
                << std::setw(18) << arg.get().getLongFlag() << std::setw(22)
                << arg.get().getTypeDescriptor() << arg.get().getHelpMessage()
                << std::endl;
    }

    std::cout << "\nResolutions:" << std::endl;

    for (auto &entry : MediaDefinedFormat::formats) {
      MediaFormat &format = entry.second;
      std::vector<std::string> crop = ListUtils::splitv(format.crop, ":");
      int pad = 10;
      int crop_width = std::stoi(crop[0]);
      int crop_height = std::stoi(crop[1]);
      int gcd = std::gcd(crop_width, crop_height);

      std::cout << tab(1) << "[" + format.name + "]" << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad) << "CRF:" << format.crf
                << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad) << "BITRATE:"
                << NumberUtils::formatNumber(format.bitrate, 2) + "K"
                << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad)
                << "MIN:" << NumberUtils::formatNumber(format.min, 2) + "K"
                << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad)
                << "MAX:" << NumberUtils::formatNumber(format.max, 2) + "K"
                << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad) << "RES:"
                << std::to_string(format.width) + ":" +
                       std::to_string(format.height)
                << std::endl;
      std::cout << std::left << tab(2) << std::setw(pad)
                << "CROP:" << format.crop << " " << "(" << (crop_width / gcd)
                << ":" << (crop_height / gcd) << ")" << std::endl;
      std::cout << std::endl;
    }
  };
};

#endif // HELP_H