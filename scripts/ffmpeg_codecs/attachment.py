import os
import json
from typing import TypedDict, Dict, List

# class AttachmentDefaults(TypedDict):
#     pass

class Attachment(TypedDict):
    # defaults: AttachmentDefaults
    name: str
    displayName: str
    aliases: List[str]

Attachments = Dict[str, Attachment]

def generate(source: str, destination: str) :
    """
    Populates the destination directory with attachment information
    from the source file and returns the Attachments.

    Args:
        source (str): The path to the source attachments json.
        destination (str): The path to the destination directory.
    
    Returns:
    """
    with open(source) as file:
        data: Attachments = json.load(file)

    for key, value in data.items():

        class_name: str = key.upper()
        j: str = ", "

        aliases = j.join([f'"{alias}"' for alias in value['aliases']])

        body: str = f"""#ifndef ATTACHMENT_{class_name}_GENERATED_H
#define ATTACHMENT_{class_name}_GENERATED_H

#include "BaseAttachment.h"
#include <set>
#include <string>
#include <vector>

class Attachment_{class_name}_Generated : public BaseAttachment {{
 public:
  Attachment_{class_name}_Generated():
                 BaseAttachment() {{}};


  auto getName() const -> const std::string override {{ return "{value['name']}"; }};
  auto getDisplayName() const -> const std::string override {{ return "{value['displayName']}"; }};
  auto getAliases() const -> const std::set<std::string> override {{ return {{{aliases}}}; }};

}};

#endif // ATTACHMENT_{class_name}_GENERATED_H"""

        # check if the file exists
        if (not os.path.exists(os.path.join(destination, f"Attachment_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"Attachment_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)
            
            print(f"Attachment: {class_name}... Created.")
        else:
            with open(os.path.join(destination, f"Attachment_{class_name}_Generated.h"), "r") as existing_file:
                existing_content = existing_file.read()
            if existing_content != body:
                with open(os.path.join(destination, f"Attachment_{class_name}_Generated.h"), "w") as out_file:
                    out_file.write(body)
                print(f"Attachment: {class_name}... Updated.")
            #else:
            #    print(f"Attachment: {class_name}... Skipped (up to date).")

    return data
