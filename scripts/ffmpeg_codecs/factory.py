import os

def generate(
        class_prefix: str, 
        generic_class: str, 
        data: list[str], 
        destination: str):
    """
    Generates a factory implementation file in the destination directory.

    Args:
        header_prefix (str): The prefix for the header guard.
        class_prefix (str): The prefix for the class names.
        generic_class (str): The generic class type for the factory.
        data (List[str]): The audio codecs data.
        destination (str): The path to the destination directory.
    """
    includes: str = ""
    initBody: str = ""

    for key in data:
        class_name: str = key.upper()
        includes += f'#include "{class_prefix}_{class_name}_Generated.h"\n'
        initBody += f"""
  auto {key} = []() -> {generic_class}* {{
    return new {class_prefix}_{class_name}_Generated();
  }};
  for (auto &alias : {class_prefix}_{class_name}_Generated().getAliases()) {{
    codec_registry.add(
      alias,
      std::make_unique<std::function<{generic_class}*()>>({key}));
  }}
"""

    body = f"""#include "{class_prefix}Factory.h"
{includes}
#include <functional>
#include <string>

Registry<std::string, std::function<{generic_class} *()>> 
    {class_prefix}Factory::codec_registry;

auto {class_prefix}Factory::create(const std::string &name) 
    -> {generic_class} * {{
  if (codec_registry.empty()) {{
    initialize();
  }}

  auto result = codec_registry.get(name);

  if (result != nullptr) {{
    return (*result)();
  }}

  return nullptr;
}}

auto {class_prefix}Factory::registry()
    -> const Registry<std::string, std::function<{generic_class} *()>> & {{
  if (codec_registry.empty()) {{
    initialize();
  }}

  return codec_registry;
}}

auto {class_prefix}Factory::initialize() -> void {{
{initBody}
}}"""

    if (not os.path.exists(os.path.join(destination, f"{class_prefix}Factory.cpp"))):
        with open(os.path.join(destination, f"{class_prefix}Factory.cpp"), "w") as out_file:
            out_file.write(body)
            print(f"{class_prefix}Factory.cpp... Created.")
    else:
        with open(os.path.join(destination, f"{class_prefix}Factory.cpp"), "r") as existing_file:
            existing_content = existing_file.read()
        if existing_content != body:
            with open(os.path.join(destination, f"{class_prefix}Factory.cpp"), "w") as out_file:
                out_file.write(body)
            print(f"{class_prefix}Factory.cpp... Updated.")
        else:
            print(f"{class_prefix}Factory.cpp... Skipped (up to date).")
