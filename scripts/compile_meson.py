import os
import sys
from pathlib import Path

def readDirRecursive(directory):

    # exit if invalid path
    if not Path(directory).exists():
        print(f"Directory '{directory}' does not exist.")
        exit()

    root = directory
    built = f"# root/{root}meson.build\n\n"

    for file in Path(directory).glob("*"):
        
        path = root + file
        
        # if dir, recursive
        if file.is_dir:
            built += f"subdir('{file.name}')\n"
            readDirRecursive(path + '/')
            continue

        # ignore non cpp files
        if not file.name.endswith(".cpp"):
            continue

        # first iteration has to define src_files
        if file.name.lower() == "main.cpp":
            built += f"src_files = files('{file.name}')\n"
        else:
            built += f"src_files += files('{file.name}')\n"

    build_file = Path(root + "meson.build")

    if build_file.exists() and build_file.is_file():
        build_file_content = build_file.read_text()
        if build_file_content != built:
            build_file.write_text(built)
            print(f"meson.build... Updated.")
        else:
            print(f"meson.build... Skipped (up to date).")
    else:
        build_file.write_text(built)
        print(f"meson.build... Created.")

readDirRecursive(sys.argv[1])
print("Meson OK.")