# root/meson.build
project('Convert-CPP', 'cpp', default_options: ['cpp_std=c++20'])

# Include directories (like nlohmann)
external_dir = include_directories('external')

# Add the src directory (where the main build configuration is done)
subdir('src')

# Define the output executable and install it in the root directory
executable(
    'convert',
    src_files,
    include_directories: [external_dir],
    install: true,
    install_dir: meson.current_source_dir(),
) # Place the binary in the root folder
