import os
import json
from typing import TypedDict, Dict, List

class ContainerDefaults(TypedDict):
    audioCodec: str
    subtitleCodec: str
    videoCodec: str

class Container(TypedDict):
    defaults: ContainerDefaults
    name: str
    displayName: str
    aliases: List[str]
    supportedAudioCodecs: List[str]
    supportedSubtitleCodecs: List[str]
    supportedVideoCodecs: List[str]

Containers = Dict[str, Container]


def generate(source: str, destination: str):
    """
    Populates the destination directory with container information
    from the source file.

    Args:
        source (str): The path to the source containers json.
        destination (str): The path to the destination directory.
    """
    with open(source) as file:
        data: Containers = json.load(file)

    for key, value in data.items():

        class_name: str = value['name'].upper()
        defaults: ContainerDefaults = value['defaults']

        aliases = ", ".join([f'"{alias}"' for alias in value['aliases']])
        supportedAudioCodecs = ", ".join([f'"{codec}"' for codec in value['supportedAudioCodecs']])
        supportedSubtitleCodecs = ", ".join([f'"{codec}"' for codec in value['supportedSubtitleCodecs']])
        supportedVideoCodecs = ", ".join([f'"{codec}"' for codec in value['supportedVideoCodecs']])

        body: str = f"""#ifndef CONTAINER_{class_name}_GENERATED_H
#define CONTAINER_{class_name}_GENERATED_H

#include "BaseContainer.h"
#include <vector>
#include <string>

class Container_{class_name}_Generated : public BaseContainer {{
public:

    auto getName() const -> const std::string override {{
        return "{value['name']}";
    }}

    auto getDisplayName() const -> const std::string override {{
        return "{value['displayName']}";
    }}

    auto getAliases() const -> const std::set<std::string> override {{
        return {{{aliases}}};
    }}

    auto supportedAudioCodecs() const -> const std::vector<std::string> override {{
        return {{{supportedAudioCodecs}}};
    }}

    auto supportedSubtitleCodecs() const -> const std::vector<std::string> override {{
        return {{{supportedSubtitleCodecs}}};
    }}

    auto supportedVideoCodecs() const -> const std::vector<std::string> override {{
        return {{{supportedVideoCodecs}}};
    }}

    auto fallbackAudioCodec() const -> const std::string override {{
        return "{defaults['audioCodec']}";
    }}

    auto fallbackSubtitleCodec() const -> const std::string override {{
        return "{defaults['subtitleCodec']}";
    }}

    auto fallbackVideoCodec() const -> const std::string override {{
        return "{defaults['videoCodec']}";
    }}
}};

#endif // CONTAINER_{class_name}_GENERATED_H
"""

        # check if the file exists
        if (not os.path.exists(os.path.join(destination, f"Container_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"Container_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)

            print(f"Container: {value['name']}... Created.")
        else:
            with open(os.path.join(destination, f"Container_{class_name}_Generated.h"), "r") as existing_file:
                existing_content = existing_file.read()
            if existing_content != body:
                with open(os.path.join(destination, f"Container_{class_name}_Generated.h"), "w") as out_file:
                    out_file.write(body)
                print(f"Container: {value['name']}... Updated.")
            #else:
            #    print(f"Container: {value['name']}... Skipped (up to date).")

    return data
