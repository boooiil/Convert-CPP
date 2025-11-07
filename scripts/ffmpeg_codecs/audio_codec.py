import os
import json
from typing import TypedDict, Dict, List

class AudioCodecDefaults(TypedDict):
    channel: int
    sampleRate: int
    bitDepth: int

class AudioCodec(TypedDict):
    defaults: AudioCodecDefaults
    name: str
    displayName: str
    aliases: List[str]
    supportedChannels: List[int]
    supportedSampleRates: List[int]
    supportedBitDepths: List[int]

AudioCodecs = Dict[str, AudioCodec]


def generate(source: str, destination: str) :
    """
    Populates the destination directory with audio codec information
    from the source file and returns the AudioCodecs.

    Args:
        source (str): The path to the source audio codecs json.
        destination (str): The path to the destination directory.
    
    Returns:
    """
    with open(source) as file:
        data: AudioCodecs = json.load(file)

    for key, value in data.items():

        class_name: str = value['name'].upper()
        defaults: AudioCodecDefaults = value['defaults']
        j: str = ", "

        aliases = j.join([f'"{alias}"' for alias in value['aliases']])
        supportedChannels = j.join([str(n) for n in value['supportedChannels']])
        supportedSampleRates = j.join([str(n) for n in value['supportedSampleRates']])
        supportedBitDepths = j.join([str(n) for n in value['supportedBitDepths']])

        body: str = f"""#ifndef AUDIO_CODEC_{class_name}_GENERATED_H
#define AUDIO_CODEC_{class_name}_GENERATED_H

#include "BaseAudioCodec.h"
#include <set>
#include <string>
#include <vector>

class AudioCodec_{class_name}_Generated : public BaseAudioCodec {{
 public:
  AudioCodec_{class_name}_Generated(int channel = {defaults['channel']},
                 int sampleRate = {defaults['sampleRate']},
                 int bitDepth = {defaults['bitDepth']}):
                 BaseAudioCodec(channel, sampleRate, bitDepth) {{}};

  auto getName() -> std::string override {{ return "{value['name']}"; }};
  auto getDisplayName() -> std::string override {{ return "{value['displayName']}"; }};
  auto getAliases() -> std::set<std::string> override {{ return {{{aliases}}}; }};

  auto supportedChannels() -> std::vector<int> override {{ return {{{supportedChannels}}}; }};
  auto supportedSampleRates() -> std::vector<int> override {{ return {{{supportedSampleRates}}}; }};
  auto supportedBitDepths() -> std::vector<int> override {{ return {{{supportedBitDepths}}}; }};

  auto fallbackChannel() -> int override {{ return {defaults['channel']}; }};
  auto fallbackSampleRate() -> int override {{ return {defaults['sampleRate']}; }};
  auto fallbackBitDepth() -> int override {{ return {defaults['bitDepth']}; }};

}};

#endif // AUDIO_CODEC_{class_name}_GENERATED_H"""

        # check if the file exists
        if (not os.path.exists(os.path.join(destination, f"AudioCodec_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"AudioCodec_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)
            
            print(f"Audio Codec: {value['name']}... Created.")
        else:
            with open(os.path.join(destination, f"AudioCodec_{class_name}_Generated.h"), "r") as existing_file:
                existing_content = existing_file.read()
            if existing_content != body:
                with open(os.path.join(destination, f"AudioCodec_{class_name}_Generated.h"), "w") as out_file:
                    out_file.write(body)
                print(f"Audio Codec: {value['name']}... Updated.")
            else:
                print(f"Audio Codec: {value['name']}... Skipped (up to date).")

    return data
