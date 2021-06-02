#ifndef THREEDIM_CAMERA
#define THREEDIM_CAMERA
#include <variant>
#include "geometry.hpp"

namespace td {
  struct screen {
    float distance_from_screen_to_camera;
    point bottom_left, bottom_right, top_left;
  };

  std::vector<std::vector<raytrace_result>>
  shoot(const screen & scr, int xres, int yres, const std::vector<colored_triangle> & cts);
}

#endif
