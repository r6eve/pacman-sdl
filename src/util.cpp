#include <math.h>

namespace util {

int get_distance(int x1, int y1, int x2, int y2) {
  return (int)hypot((double)(x1 - x2), (double)(y1 - y2));
}

}  // namespace util
