#include <math.h>

namespace util {

unsigned int get_distance(int x1, int y1, int x2, int y2) noexcept {
  return static_cast<unsigned int>(
      hypot(static_cast<double>(x1 - x2), static_cast<double>(y1 - y2)));
}

}  // namespace util
