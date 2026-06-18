import sys
from pathlib import Path

def readDirRecursive(directory: Path):
    if not directory.exists():
        print(f"Directory '{directory}' does not exist.")
        sys.exit()

    root = directory
    built = f"# root/{root.as_posix()}/meson.build\n\n"

    subdirs = []
    cpp_files = []

    for file in Path(root).glob("*"):
        path: Path = root / file.name
        if file.is_dir():
            subdirs.append(file.name)
            readDirRecursive(path)
        elif file.name.endswith(".cpp"):
            cpp_files.append(file.name)

    for name in cpp_files:
        if name.lower() == "main.cpp":
            built += f"src_files = files('{name}')\n"
        else:
            built += f"src_files += files('{name}')\n"

    for name in subdirs:
        built += f"subdir('{name}')\n"

    build_file = Path(root / "meson.build")
    if build_file.exists() and build_file.is_file():
        build_file_content = build_file.read_text()
        if build_file_content != built:
            build_file.write_text(built)
            print(f"meson.build... Updated.")
    else:
        build_file.write_text(built)
        print(f"meson.build... Created.")

if len(sys.argv) != 2:
    print("Usage: python compile_meson.py <directory>")
    sys.exit()

readDirRecursive(Path(sys.argv[1]))
print("Meson OK.")