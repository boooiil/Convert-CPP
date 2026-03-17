import os
import json
from pathlib import Path
from typing import TypedDict, Dict, List, NotRequired

class Definition(TypedDict):
    name: str
    aliases: List[str]
    short_description: str
    long_description: str

class EnumAssociation(TypedDict):
    enum: str | None
    header: str | None


Definitions = Dict[str, Definition]
FileStruct = Dict[tuple[str | None, str | None], Definitions]

def generate(
        definitions_dir: str,
        destination: str
):
    """
    Generates a factory implementation file in the destination directory.

    Args:
        definitions_dir (str): The path to the definitions directory.
        destination (str): The path to the destination directory.
    """

    data: FileStruct = {}
    flat: Definitions = {}
    header = None
    enum = None

    for file in Path(definitions_dir).glob("*.json"):
        with open(file, "r", encoding="utf-8") as f:
            raw = json.load(f)

            header = raw.pop("header", None)
            enum = raw.pop("enum", None)

            enum_assoc: tuple[str | None, str | None] = (header, enum)
            file_defs: Definitions = raw

            if enum_assoc not in data:
                data[enum_assoc] = file_defs
            else:
                data[enum_assoc].update(file_defs)
               

    for (header, enum), definitions in data.items():
      flat.update(definitions)
      
      for index, (c_name, definition) in enumerate(definitions.items()):
        
        class_name: str = c_name.upper()
        j: str = ", "
        aliases = j.join([f'"{alias}"' for alias in definition['aliases']]) + f", \"{definition['name']}\""
        includes = ""

        if header != None and enum != None:
          includes = f'#include "src/program/settings/enums/{header}.h"\n'
          closing = f""",
                 const int enum_assoc = {index},
                 const std::type_info &enum_type = typeid({header}::{enum})):
                 Definition(name, aliases, short_description, long_description, enum_assoc, enum_type) {{}};"""
        else:
          closing = f"""):
                 Definition(name, aliases, short_description, long_description) {{}};"""
         

        body: str = f"""#ifndef DEFINITION_{class_name}_GENERATED_H
#define DEFINITION_{class_name}_GENERATED_H

{includes}#include "Definition.h"
#include <string>
#include <unordered_set>

class Definition_{class_name}_Generated : public Definition {{
 public:
  Definition_{class_name}_Generated(const std::string& name = "{definition['name']}",
                 const std::unordered_set<std::string>& aliases = {{{aliases}}},
                 const std::string& short_description = "{definition['short_description']}",
                 const std::string& long_description = "{definition['long_description']}"{closing}

}};

#endif // DEFINITION_{class_name}_GENERATED_H"""
    
        if (not os.path.exists(os.path.join(destination, f"Definition_{class_name}_Generated.h"))):
            with open(os.path.join(destination, f"Definition_{class_name}_Generated.h"), "w") as out_file:
              out_file.write(body)
            print(f"Definition_{class_name}_Generated.h... Created.")
        else:
            with open(os.path.join(destination, f"Definition_{class_name}_Generated.h"), "r") as existing_file:
              existing_content = existing_file.read()
            if existing_content != body:
              with open(os.path.join(destination, f"Definition_{class_name}_Generated.h"), "w") as out_file:
                out_file.write(body)
              print(f"Definition_{class_name}_Generated.h... Updated.")
            #else:
            #  print(f"Definition_{class_name}_Generated.h... Skipped (up to date).")
      
    return flat


    