.PHONY: validate_meson linux_validate_meson linux_validate_meson_builddir win_validate_meson win_validate_meson_builddir copy_build generate clean valgrind debug run
# Absolute path to the directory where this Makefile lives
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# file builds to builddir/
COPY_DEST := $(MAKEFILE_DIR)
COMPILE_TYPE := debug

ifeq ($(OS),Windows_NT)
    PLATFORM := windows
	OUTPUT_NAME := convert.exe
else
    PLATFORM := linux
	OUTPUT_NAME := convert
endif

RUN_DEST := ./$(OUTPUT_NAME)

compile: generate
	@meson compile -C build-$(COMPILE_TYPE)
	@$(MAKE) -s copy_build COMPILE_TYPE=$(COMPILE_TYPE)

linux_validate_meson:
	@if ! command -v meson >/dev/null 2>&1; then \
		echo "Meson build system is not installed. Please install it to proceed."; \
		exit 1; \
	@else \
	    echo "Meson is installed."; \
	fi

linux_validate_meson_builddir:
	@if [ ! -d "build-debug" ]; then \
		meson setup build-debug --buildtype debug; \
	else \
		echo "Build directory 'build-debug' exists."; \
	fi
	@if [ ! -d "build-release" ]; then \
		meson setup build-release --buildtype release; \
	else \
		echo "Build directory 'build-release' exists."; \
	fi

win_validate_meson:
	@where meson >nul 2>nul || ( \
		echo Meson build system is not installed. Please install it to proceed. & \
		exit 1 \
	)
	@echo Meson is installed.

win_validate_meson_builddir:
	@if not exist build-debug ( \
		meson setup build-debug --buildtype debug \
	) else ( \
		echo Build directory 'build-debug' exists. \
	)
	@if not exist build-release ( \
		meson setup build-release --buildtype release \
	) else ( \
		echo Build directory 'build-release' exists. \
	)

validate_meson:
ifeq ($(PLATFORM),windows)
	@echo Detected Windows platform.
	@$(MAKE) -s win_validate_meson
else
	@echo Detected Linux platform.
	@$(MAKE) -s linux_validate_meson
endif

validate_meson_builddir: validate_meson
ifeq ($(PLATFORM),windows)
	@$(MAKE) -s win_validate_meson_builddir
else
	@$(MAKE) -s linux_validate_meson_builddir
endif

generate: validate_meson_builddir
	python scripts/populate_ffmpeg_types.py ./
	node scripts/compile_meson.js

copy_build:
	@echo Copying build files to $(COPY_DEST)
	@python -c "import shutil, os; shutil.copyfile('build-$(COMPILE_TYPE)/$(OUTPUT_NAME)', os.path.join('$(COPY_DEST)', '$(OUTPUT_NAME)'))"

clean:
	meson compile -C build-$(COMPILE_TYPE) --clean

valgrind: compile
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(RUN_DEST) $(DEBUG_ARGS)

debug: compile
	$(RUN_DEST) $(DEBUG_ARGS)

run: compile
	$(RUN_DEST) $(ARGS)
