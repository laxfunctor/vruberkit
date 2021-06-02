#include "color.hpp"

namespace td {
  color scale_color(const color & a, const color & c){
    return {a.red * c.red, a.green * c.green, a.blue * c.blue};
  }
}
