#include <getopt.h>
#include <iostream>
#include "pacman.hpp"

using namespace std;

bool parse_options(const int argc, char **argv) noexcept {
  bool ret = false;
  opterr = 0;
  const option long_options[] = {
      {"debug", no_argument, nullptr, 'd'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
  };

  for (;;) {
    const int curind = optind;
    const int c = getopt_long(argc, argv, "dh", long_options, nullptr);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        cout << R"(Usage: pacman-sdl [options]

Options:
    -d  --debug         debug mode
    -h, --help          print this help menu
)";
        exit(EXIT_SUCCESS);
      case 'd':
        ret = true;
        break;
      case '?': {
        string av(argv[curind]);
        int n = 0;
        while (av[n] == '-') {
          ++n;
        }
        av.erase(av.begin(), av.begin() + n);
        cerr << "Unrecognized option: '" << av << "'\n";
        exit(EXIT_FAILURE);
      }
      default:
        // do nothing
        break;
    }
  }

  return ret;
}

int main(int argc, char **argv) {
  const bool debug_mode = parse_options(argc, argv);
  run(debug_mode);
  // NOTREACHED
}
