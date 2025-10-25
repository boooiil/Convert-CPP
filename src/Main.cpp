#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <windows.h>

#include "./utils/signals/WindowsSignalHandler.h"

using SignalHandler = WindowsSignalHandler;

#else
#include "./utils/signals/UnixSignalHandler.h"

using SignalHandler = UnixSignalHandler;

#endif

#include <exception>
#include <iostream>

#include "program/Program.h"

/**
 * WALK AWAY NOTES:
 *
 * We need to iterate over all of the subtitle streams and see if they can be
 mapped to the desired codecs.
 * If a stream cannot be mapped, we need to decide whether to drop it or use a
 fallback codec.

 * We are creating a lot of memory by allocating for each new
 * supported codec (BaseContainer). We should probably make a registry of
 * supported video, audio, and subtitle codecs that we can pull from instead of
 * creating new instances.
 *
 * this also lets us use equality using pointer addresses when validating, which
 * can be fun
 *
 * add option for modifying audio codec
 * add option for modifying audio channels
 *
 * create argument factory
 *
 * av1 does not support film tune
 *
 * Determine how to handle a parent class and how we
 * will be storing the data
 *
 */

/**
 * Why are we looking for an external while when using convert?
 *
 * Why not just use a subdir flag or something such that -subdir 1 or equivalent
 * recursively searches? This would let us not use a parent/child method for
 * handling the processes.
 *
 * Pros:
 *  - Do not need to create a separate definition to handle the parent objects.
 *  - Media rename already handles adjusting paths for directories so there is
 * no additional consideration.
 *  - Do not need to create a threaded process for each parent-child process.
 * (we would have 1*amount threads for convert and 1*amount threads for parent)
 * Cons:
 *  - ??
 */

/**
 * FINISH NOW, OPTIMIZE LATER
 * FINSIH NOW, OPTOIMIZE LATRE
 * FIFNEIH NOW, OETINMEN LENTER
 *
 * TODO: DOCUMENT, DOCUMENT, DOCUMENT
 */

/**
 * @brief Main
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return int
 */
int main(int argc, char *argv[]) {
#ifdef _DEBUG
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  // _CrtSetBreakAlloc(15475);

#endif

  Program program = Program();

  try {
    SignalHandler sigHandler = SignalHandler();
    sigHandler.registerHandler();

    program.prepare(argc, argv);

    program.run();

  } catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  program.end();

  return 0;
}
