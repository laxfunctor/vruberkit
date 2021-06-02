#ifndef THREEDIM_GEOMETRY
#define THREEDIM_GEOMETRY
#include <tuple>
#include <variant>
#include <vector>
#include <optional>
#include "color.hpp"

namespace td {
  typedef std::tuple<float, float, float> point;
  const point zero_vec = {0, 0, 0};

  typedef struct {
    point pt; // start point
    point dir; // direction vector
  } line;

  typedef std::tuple<point, point, point> triangle;

  typedef char surface_kind;
  const surface_kind EMIT = 0;
  const surface_kind ABSORB = 1;
  typedef std::tuple<surface_kind, color, triangle> colored_triangle;

  struct Absorbed { }; // if ray reflects many times
  struct Diverge { }; // if ray goes to void
  struct Collide { color c; }; // if ray collides light
  typedef std::variant<Absorbed, Diverge, Collide> raytrace_result;

  bool equal(float a, float b);
  point cross_product(const point & u, const point & v);
  point operator + (const point & u, const point & v);
  point operator - (const point & u, const point & v);
  float inner_product(const point & u, const point & v);
  float norm(const point & v);
  float distance(const point & p, const point & q);
  float area(const triangle & t);
  bool point_in_triangle(const point & p, const triangle & t);
  point scale(float a, const point & p);
  point get_point_on_line(const line & l, float a);
  std::pair<point, float> point_line_foot_of_perpendicular(const point & p, const line & l);
  point normalize(const point & v);
  line normalize(const line & l);
  line normal_vector(const triangle & t);

  point            rotate_x(const double theta, const point & p,             const point & o = zero_vec);
  triangle         rotate_x(const double theta, const triangle & t,          const point & o = zero_vec);
  colored_triangle rotate_x(const double theta, const colored_triangle & ct, const point & o = zero_vec);
  point            rotate_y(const double theta, const point & p,             const point & o = zero_vec);
  triangle         rotate_y(const double theta, const triangle & t,          const point & o = zero_vec);
  colored_triangle rotate_y(const double theta, const colored_triangle & ct, const point & o = zero_vec);
  point            rotate_z(const double theta, const point & p,             const point & o = zero_vec);
  triangle         rotate_z(const double theta, const triangle & t,          const point & o = zero_vec);
  colored_triangle rotate_z(const double theta, const colored_triangle & ct, const point & o = zero_vec);

  std::pair<float, line> reflect_on_surface(const line & ray, const line & n);
  std::optional<std::pair<float, line>> intersection(const line & ray, const triangle & t);

  std::tuple<std::optional<std::pair<float, line>>, surface_kind, color>
  reflect(const line & ray, const colored_triangle & colored_triangle);
  std::optional<std::tuple<line, surface_kind, color>>
  reflect(const line & ray, const std::vector<colored_triangle> & cts);
  raytrace_result raytrace(const line & ray,
                           const std::vector<colored_triangle> & cts,
                           unsigned int max_reflection_n);
}

#endif
