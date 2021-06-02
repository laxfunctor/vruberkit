#include <cmath>
#include <optional>
#include <vector>
#include <algorithm>
#include <cmath>

#include "color.hpp"
#include "geometry.hpp"

namespace td {
  const float eps = 1e-2;

  bool equal(float a, float b) {
    return (1 - eps <= a / b) && (a / b <= 1 + eps);
  }

  point cross_product(const point & u, const point & v) {
    const auto [ux, uy, uz] = u;
    const auto [vx, vy, vz] = v;
    return {uy * vz - uz * vy,
            uz * vx - ux * vz,
            ux * vy - uy * vx};
  }

  point operator + (const point & u, const point & v) {
    const auto [ux, uy, uz] = u;
    const auto [vx, vy, vz] = v;
    return {ux + vx, uy + vy, uz + vz};
  }

  point operator - (const point & u, const point & v) {
    const auto [ux, uy, uz] = u;
    const auto [vx, vy, vz] = v;
    return {ux - vx, uy - vy, uz - vz};
  }

  float inner_product(const point & u, const point & v) {
    const auto [ux, uy, uz] = u;
    const auto [vx, vy, vz] = v;
    return ux * vx + uy * vy + uz * vz;
  }

  float norm(const point & v) {
    return std::sqrt( inner_product(v, v) );
  }

  float distance(const point & p, const point & q) {
    return norm(q - p);
  }

  float area(const triangle & t) {
    const auto [p0, p1, p2] = t;
    return norm( cross_product(p1 - p0, p2 - p0) );
  }

  bool point_in_triangle(const point & p, const triangle & t) {
    const auto [p0, p1, p2] = t;
    float s = area(t);
    float s0 = area({p, p1, p2});
    float s1 = area({p0, p, p2});
    float s2 = area({p0, p1, p});
    return equal(s, s0 + s1 + s2);
  }

  point scale(float a, const point & p) {
    const auto [x, y, z] = p;
    return {a * x, a * y, a * z};
  }

  point get_point_on_line(const line & l, float a) {
    return l.pt + scale(a, l.dir);
  }

  std::pair<point, float> point_line_foot_of_perpendicular(const point & p, const line & l) {
    float a = inner_product(l.dir, p - l.pt) / inner_product(l.dir, l.dir);
    return {get_point_on_line(l, a), a};
  }

  point normalize(const point & v) {
    float n = distance(zero_vec, v);
    return scale(1 / n, v);
  }

  line normalize(const line & l) {
    return {l.pt, normalize(l.dir)};
  }

  line normal_vector(const triangle & t) {
    const auto [p0, p1, p2] = t;
    return {p0, cross_product(p1 - p0, p2 - p0)};
  }

  /*
   * 1           0             0
   * 0 (cos theta) (- sin theta)
   * 0 (sin theta)   (cos theta)
   */
  point rotate_x(const double theta, const point & p, const point & o) {
    const auto [x, y, z] = p - o;
    const double cos_theta = std::cos(theta);
    const double sin_theta = std::sin(theta);
    const point q = {x, y * cos_theta - z * sin_theta, y * sin_theta + z * cos_theta};
    return q + o;
  }
  /*
   *   (cos theta) 0 (sin theta)
   *             0 1           0
   * (- sin theta) 0 (cos theta)
   */
  point rotate_y(const double theta, const point & p, const point & o) {
    const auto [x, y, z] = p - o;
    const double cos_theta = std::cos(theta);
    const double sin_theta = std::sin(theta);
    const point q = {x * cos_theta + z * sin_theta, y, - x * sin_theta + z * cos_theta};
    return q + o;
  }
  /*
   * (cos theta) (- sin theta) 0
   * (sin theta)   (cos theta) 0
   *           0             0 1
   */
  point rotate_z(const double theta, const point & p, const point & o) {
    const auto [x, y, z] = p - o;
    const double cos_theta = std::cos(theta);
    const double sin_theta = std::sin(theta);
    const point q = {x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta, z};
    return q + o;
  }

  triangle rotate_x(const double theta, const triangle & t, const point & o) {
    const auto & [p, q, r] = t;
    return {rotate_x(theta, p, o), rotate_x(theta, q, o), rotate_x(theta, r, o)};
  }
  colored_triangle rotate_x(const double theta, const colored_triangle & ct, const point & o) {
    const auto & [skind, col, t] = ct;
    return {skind, col, rotate_x(theta, t, o)};
  }
  triangle rotate_y(const double theta, const triangle & t, const point & o) {
    const auto & [p, q, r] = t;
    return {rotate_y(theta, p, o), rotate_y(theta, q, o), rotate_y(theta, r, o)};
  }
  colored_triangle rotate_y(const double theta, const colored_triangle & ct, const point & o) {
    const auto & [skind, col, t] = ct;
    return {skind, col, rotate_y(theta, t, o)};
  }
  triangle rotate_z(const double theta, const triangle & t, const point & o) {
    const auto & [p, q, r] = t;
    return {rotate_z(theta, p, o), rotate_z(theta, q, o), rotate_z(theta, r, o)};
  }
  colored_triangle rotate_z(const double theta, const colored_triangle & ct, const point & o) {
    const auto & [skind, col, t] = ct;
    return {skind, col, rotate_z(theta, t, o)};
  }

  /* return: (a, l)
   *     where
   *       - ray.pt + a * ray.dir = l.pt
   *       - l is a reflected ray on the surface represented by the normal vector n.
   */
  /*
   *       mid_point
   *  _____v_____opposite
   *  \    |    /
   *   \   m   /
   * ray\  |  /l
   *     \ | /
   *      \|/
   *  _____v______surface
   *       ^reflection_point
   */
  std::pair<float, line> reflect_on_surface(const line & ray, const line & n) {
    const float a = inner_product(n.pt - ray.pt, n.dir) / inner_product(ray.dir, n.dir);
    const point reflection_point = get_point_on_line(ray, a);
    const line m = {reflection_point, n.dir};
    const point mid_point = point_line_foot_of_perpendicular(ray.pt, m).first;
    const point opposite = get_point_on_line({ray.pt, mid_point - ray.pt}, 2.0);
    return {a, {reflection_point, opposite - reflection_point}};
  }

  std::optional<std::pair<float, line>> intersection(const line & ray, const triangle & t) {
    const auto result = reflect_on_surface(ray, normal_vector(t));
    const auto [a, r] = result;
    if (point_in_triangle(r.pt, t)) {
      return result;
    }
    return std::nullopt;
  }

  std::tuple<std::optional<std::pair<float, line>>, surface_kind, color>
  reflect(const line & ray, const colored_triangle & colored_triangle) {
    const auto [sk, col, t] = colored_triangle;
    const std::optional<std::pair<float, line>> refl = intersection(ray, t);
    return {refl, sk, col};
  }

  std::optional<std::tuple<line, surface_kind, color>>
  reflect(const line & ray, const std::vector<colored_triangle> & cts) {
    bool reflected = false;
    float min_a = -1.0;
    std::tuple<line, surface_kind, color> ret;
    for (std::size_t i = 0; i < cts.size(); i++) {
      const auto [refl, sk, col] = reflect(ray, cts[i]);
      if (refl) {
        const auto [a, new_ray] = refl.value();
        if (reflected) {
          if (eps < a && a < min_a) {
            min_a = a;
            ret = std::tuple<line, surface_kind, color>(new_ray, sk, col);
          }
        } else if(min_a < 0 && eps < a) {
          reflected = true;
          min_a = a;
          ret = std::tuple<line, surface_kind, color>(new_ray, sk, col);
        }
      }
    }
    if (reflected) {
      return ret;
    }
    return std::nullopt;
  }

  raytrace_result raytrace(const line & ray_,
                           const std::vector<colored_triangle> & cts,
                           unsigned int max_reflection_n){
    line ray = ray_;
    unsigned int i = 0;
    color result_color = white;
    for (; i < max_reflection_n; i++) {
      const auto refl = reflect(ray, cts);
      if (refl) {
        const auto [new_ray, sk, c] = refl.value();
        result_color = scale_color(c, result_color);
        if (sk == EMIT) {
          break;
        }
        ray = new_ray;
      } else {
        return Diverge{};
      }
    }
    if (i == max_reflection_n) {
      return Absorbed{};
    }

    return Collide{ result_color };
  }
}

