import os
import json
from typing import TypedDict, Dict, List

class VideoCodecDefaults(TypedDict):
    runningPreset: str
    runningLevel: str
    runningTune: str

class VideoCodec(TypedDict):
    defaults: VideoCodecDefaults
    name: str
    displayName: str
    controlRateFlag: str
    aliases: List[str]
    supportedPresets: List[str]
    supportedLevels: List[str]
    supportedTunes: List[str]

VideoCodecs = Dict[str, VideoCodec]

def generate(source: str, destination: str):
    """
    Populates the destination directory with video codec information
    from the source file.

    Args:
        source (str): The path to the source video codecs json.
        destination (str): The path to the destination directory.
    """
    with open(source) as file:
        data: VideoCodecs = json.load(file)

    for key, value in data.items():

        class_name: str = value['name'].upper()
        defaults: VideoCodecDefaults = value['defaults']

        aliases = ", ".join([f'"{alias}"' for alias in value['aliases']])
        supportedPresets = ", ".join([f'"{preset}"' for preset in value['supportedPresets']])
        supportedLevels = ", ".join([f'"{level}"' for level in value['supportedLevels']])
        supportedTunes = ", ".join([f'Tunes_N::{tune}' for tune in value['supportedTunes']])

        body: str = f"""#ifndef VIDEO_CODEC_{class_name}_GENERATED_H
#define VIDEO_CODEC_{class_name}_GENERATED_H

#include "BaseVideoCodec.h"
#include <set>
#include <string>
#include <vector>

class VideoCodec_{class_name}_Generated : public BaseVideoCodec {{
 public:
    VideoCodec_{class_name}_Generated(std::string runningPreset = "{defaults['runningPreset']}",
                     std::string runningLevel = "{defaults['runningLevel']}",
                     Tunes_N::Tunes runningTune = Tunes_N::{defaults['runningTune']}):
                     BaseVideoCodec(runningPreset, runningLevel, runningTune) {{}};
    
    auto getName() -> std::string override {{ return "{value['name']}"; }};
    auto getDisplayName() -> std::string override {{ return "{value['displayName']}"; }};
    auto getAliases() -> std::set<std::string> override {{ return {{{aliases}}}; }};
    
    auto controlRateFlag() -> std::string override {{ return "{value['controlRateFlag']}"; }};
    auto supportedPresets() -> std::vector<std::string> override {{ return {{{supportedPresets}}}; }};
    auto supportedLevels() -> std::vector<std::string> override {{ return {{{supportedLevels}}}; }};
    auto supportedTunes() -> std::vector<Tunes_N::Tunes> override {{ return {{{supportedTunes}}}; }};
    
    auto fallbackPreset() -> std::string override {{ return "{defaults['runningPreset']}"; }};
    auto fallbackLevel() -> std::string override {{ return "{defaults['runningLevel']}"; }};
    auto fallbackTune() -> Tunes_N::Tunes override {{ return Tunes_N::{defaults['runningTune']}; }};
}};

#endif // VIDEO_CODEC_{class_name}_GENERATED_H
"""
        
        # check if the file exists
        if (not os.path.exists(os.path.join(destination, f"VideoCodec_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"VideoCodec_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)

            print(f"Video Codec: {value['name']}... Created.")
        else:
            with open(os.path.join(destination, f"VideoCodec_{class_name}_Generated.h"), "r") as existing_file:
                existing_content = existing_file.read()
            if existing_content != body:
                with open(os.path.join(destination, f"VideoCodec_{class_name}_Generated.h"), "w") as out_file:
                    out_file.write(body)
                print(f"Video Codec: {value['name']}... Updated.")
            else:
                print(f"Video Codec: {value['name']}... Skipped (up to date).")

    return data
