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
 *
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
