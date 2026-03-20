import sys
from pathlib import Path

def readDirRecursive(directory: Path):

    # exit if invalid path
    if not directory.exists():
        print(f"Directory '{directory}' does not exist.")
        sys.exit()

    root = directory
    built = f"# root/{root.as_posix()}/meson.build\n\n"

    for file in Path(root).glob("*"):
        
        path: Path = root / file.name
        
        # if dir, recursive
        if file.is_dir():
            built += f"subdir('{file.name}')\n"
            readDirRecursive(path)
            continue

        # ignore non cpp files
        if not file.name.endswith(".cpp"):
            continue

        # first iteration has to define src_files
        if file.name.lower() == "main.cpp":
            built += f"src_files = files('{file.name}')\n"
        else:
            built += f"src_files += files('{file.name}')\n"

    build_file = Path(root / "meson.build")

    if build_file.exists() and build_file.is_file():
        build_file_content = build_file.read_text()
        if build_file_content != built:
            build_file.write_text(built)
            print(f"meson.build... Updated.")
        else:
            # print(f"meson.build... Skipped (up to date).")
            pass
    else:
        build_file.write_text(built)
        print(f"meson.build... Created.")


if (len(sys.argv) != 2):
    print("Usage: python compile_meson.py <directory>")
    sys.exit()

readDirRecursive(Path(sys.argv[1]))
print("Meson OK.")