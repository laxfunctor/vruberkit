#include <vector>
#include "color.hpp"
#include "geometry.hpp"
#include "camera.hpp"

namespace td {
  std::pair<point, point> screen_vectors(const screen & scr) {
    return {scr.bottom_right - scr.bottom_left, scr.top_left - scr.bottom_left};
  }

  /*    ._________.
   *  u |         |
   *    |  scr    |
   *    |         |
   *    |_________|
   * .__/__.     v
   * | /   |\camera_position
   * |/____|
   * ^w
   *
   */
  point camera_position(const screen & scr) {
    const auto [v, u] = screen_vectors(scr);
    const point w = scale(scr.distance_from_screen_to_camera,
                          normalize(cross_product(v, u)));
    return scr.bottom_left + scale(0.5, v + u) + w;
  }

  point screen_position(const screen & scr, float x, float y) {
    const auto [v, u] = screen_vectors(scr);
    return scr.bottom_left + scale(x, v) + scale(y, u);
  }

  std::vector<std::vector<raytrace_result>>
  shoot(const screen & scr, int xres, int yres, const std::vector<colored_triangle> & cts) {
    const auto [v, u] = screen_vectors(scr);
    const float xunit = 1.0 / xres;
    const float yunit = 1.0 / yres;
    const point camera_pos = camera_position(scr);
    std::vector<std::vector<raytrace_result>>
      image(yres, std::vector<raytrace_result>(xres, Absorbed{}));
    for (std::size_t i = 0; i < (std::size_t)yres; i++) {
      const point u_ = scale(yunit * i, u);
      for (std::size_t j = 0; j < (std::size_t)xres; j++) {
        const point v_ = scale(xunit * j, v);
        const point scr_pos = scr.bottom_left + u_ + v_;
        image[i][j] = raytrace({camera_pos, scr_pos - camera_pos}, cts, 1);
      }
    }
    return image;
  }
}
