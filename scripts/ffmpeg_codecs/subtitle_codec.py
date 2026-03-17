import os
import json
from typing import TypedDict, Dict, List

class SubtitleCodecDefaults(TypedDict):
    # Placeholder for future defaults
    pass

class SubtitleCodec(TypedDict):
    defaults: SubtitleCodecDefaults
    name: str
    displayName: str
    arg: str
    aliases: List[str]
    isImage: bool

SubtitleCodecs = Dict[str, SubtitleCodec]


def generate(source: str, destination: str):
    """
    Populates the destination directory with subtitle codec information
    from the source file.

    Args:
        source (str): The path to the source subtitle codecs json.
        destination (str): The path to the destination directory.
    """
    with open(source) as file:
        data: SubtitleCodecs = json.load(file)

    for key, value in data.items():
        class_name: str = value['name'].upper()
        aliases = ", ".join([f'"{alias}"' for alias in value['aliases']])

        body: str = f"""#ifndef SUBTITLE_CODEC_{class_name}_GENERATED_H
#define SUBTITLE_CODEC_{class_name}_GENERATED_H

#include "BaseSubtitleCodec.h"
#include <set>
#include <string>

class SubtitleCodec_{class_name}_Generated : public BaseSubtitleCodec {{
 public:
    SubtitleCodec_{class_name}_Generated() : BaseSubtitleCodec() {{}};
    
    auto getName() const -> const std::string override {{ return "{value['name']}"; }};
    auto getDisplayName() const -> const std::string override {{ return "{value['displayName']}"; }};
    auto getArg() const -> const std::string override {{ return "{value['arg']}"; }};
    auto getAliases() const -> const std::set<std::string> override {{ return {{{aliases}}}; }};
    auto isImage() const -> const bool override {{ return {str(value['isImage']).lower()}; }};
}};
#endif // SUBTITLE_CODEC_{class_name}_GENERATED_H"""

        # check if the file exists
        if (not os.path.exists(os.path.join(destination, f"SubtitleCodec_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"SubtitleCodec_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)

            print(f"Subtitle Codec: {value['name']}... Created.")
        else:
            with open(os.path.join(destination, f"SubtitleCodec_{class_name}_Generated.h"), "r") as existing_file:
                existing_content = existing_file.read()
            if existing_content != body:
                with open(os.path.join(destination, f"SubtitleCodec_{class_name}_Generated.h"), "w") as out_file:
                    out_file.write(body)
                print(f"Subtitle Codec: {value['name']}... Updated.")
            #else:
            #    print(f"Subtitle Codec: {value['name']}... Skipped (up to date).")

    return data