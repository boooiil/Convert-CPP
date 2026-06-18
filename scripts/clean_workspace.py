from shutil import rmtree
from pathlib import Path
import sys

if (len(sys.argv) != 2):
    print("Usage: python clean_workspace.py <root_directory>")
    sys.exit()

root: Path = Path(sys.argv[1])

if not root.exists():
    print(f"Directory '{root}' does not exist.")
    sys.exit()
else: 
    print(f"Using path: '{root}'...")

# Clean build directory
build_dir_release = root / "build-release"
build_dir_debug = root / "build-debug"

if build_dir_release.exists() and build_dir_release.is_dir():
    print(f"Cleaning '{build_dir_release}'...")
    rmtree(build_dir_release)
else:
    print(f"'{build_dir_release}' does not exist or is not a directory.")

if build_dir_debug.exists() and build_dir_debug.is_dir():
    print(f"Cleaning '{build_dir_debug}'...")
    rmtree(build_dir_debug)
else:
    print(f"'{build_dir_debug}' does not exist or is not a directory.")

generated_paths = [
    root /"src" / "program" / "definitions",
    root / "src" / "program" / "ffmpeg" / "attachment",
    root / "src" / "program" / "ffmpeg" / "audio",
    root / "src" / "program" / "ffmpeg" / "container",
    root / "src" / "program" / "ffmpeg" / "subtitle",
    root / "src" / "program" / "ffmpeg" / "video",
    ]

def rm_recursive(path: Path):

    for item in path.iterdir():

        # if dir, recurse
        if item.is_dir():
            rm_recursive(item)

        if item.name.endswith("meson.build"):
            print(f"Removing meson build file: '{item}'...")
            item.unlink()

        if item.parent in generated_paths:
            if item.name.endswith("Factory.cpp"):
                print(f"Removing generated factory '{item}'...")
                item.unlink()
            elif item.name.endswith("Generated.h"):
                print(f"Removing generated file '{item}'...")
                item.unlink()

if (root / "src").exists():
    rm_recursive(root / "src")
else:
    print(f"{root}/src does not exist, skipping.")

print("Workspace cleaned.")
