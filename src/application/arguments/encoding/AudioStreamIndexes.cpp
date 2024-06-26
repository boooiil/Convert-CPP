#include "./AudioStreamIndexes.h"

#include "../../../logging/Log.h"
#include "../../../utils/ListUtils.h"

AudioStreamIndexes::AudioStreamIndexes(void) : VectorArgument<int>() {
  helpMessage = "Indexes of audio streams to be encoded. Example: 0,1,2";
};

AudioStreamIndexes::~AudioStreamIndexes(void){};

void AudioStreamIndexes::parse(std::string argument) {
  if (argument.empty()) {
    errored = true;
    return;
  }

  try {
    for (std::string index : ListUtils::splitv(argument, ",")) {
      value.push_back(std::stoi(index));
    }
  } catch (const std::exception& e) {
    Log::debug(
        {"Failed to parse audio stream indexes: ", argument, "\n", e.what()});
    errored = true;
    return;
  }
}