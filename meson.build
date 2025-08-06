# root/meson.build
project('Convert-CPP', 'cpp', version: '1.0.0', default_options: ['cpp_std=c++20'])

nlohmann_json_dep = dependency(
    'nlohmann_json',
    version: '>=3.11.2',
    required: true,
    fallback: ['nlohmann_json', 'nlohmann_json_dep'],
)

stduuid_dep = dependency(
    'stduuid',
    version: '>=1.0.0',
    required: true,
    fallback: ['stduuid', 'stduuid_dep'],
)

# Add the src directory (where the main build configuration is done)
subdir('src')

# Define the output executable and install it in the root directory
executable(
    'convert',
    src_files,
    dependencies: [nlohmann_json_dep, stduuid_dep],
    install: true,
    install_dir: meson.current_source_dir(),
) # Place the binary in the root folder
