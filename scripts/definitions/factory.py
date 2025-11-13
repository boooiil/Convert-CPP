import os
import json
from definitions import definition
from pathlib import Path
from typing import TypedDict, Dict, List
    

def generate(
        definitions_dir: str,
        data: Dict[str, definition.Definition],
        destination: str
):
    """
    Generates a factory implementation file in the destination directory.

    Args:
        definitions_dir (str): The path to the definitions directory.
        destination (str): The path to the destination directory.
    """

    initBody: str = "definition_registry\n"
    includes: str = ""

    for k, v in data.items():
        
        class_name: str = k.upper()
        includes += f'#include "Definition_{class_name}_Generated.h"\n'

        for alias in v['aliases'] + [v['name']]:
            initBody += f"""  .add("{alias}", 
       std::make_unique<const Definition>(Definition_{class_name}_Generated()))
"""
            
    body = f"""#include "DefinitionRegistry.h"
{includes}
#include "Definition.h"
#include <string>

Registry<std::string, const Definition> 
    DefinitionRegistry::definition_registry;

auto DefinitionRegistry::initialize() -> void {{
{initBody};
}}"""
    
    if (not os.path.exists(os.path.join(destination, f"DefinitionRegistry.cpp"))):
        with open(os.path.join(destination, f"DefinitionRegistry.cpp"), "w") as out_file:
            out_file.write(body)
            print(f"DefinitionRegistry.cpp... Created.")
    else:
        with open(os.path.join(destination, f"DefinitionRegistry.cpp"), "r") as existing_file:
            existing_content = existing_file.read()
        if existing_content != body:
            with open(os.path.join(destination, f"DefinitionRegistry.cpp"), "w") as out_file:
                out_file.write(body)
            print(f"DefinitionRegistry.cpp... Updated.")
        else:
            print(f"DefinitionRegistry.cpp... Skipped (up to date).")



    