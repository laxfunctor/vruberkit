#include <vector>
#include <tuple>
#include <cmath>
#include <iostream>

float abs(float x) {
  if (x < 0) return -x;
  return x;
}

float avg(std::vector<float> & v) {
  float sum = 0;
  for (auto x : v) {
    sum += x;
  }
  return sum / v.size();
}

float sigma2(std::vector<float> & v, float avg) {
  float sum2 = 0;
  for (auto x : v) {
    sum2 += (x - avg) * (x - avg);
  }
  return sum2 / v.size();
}

float cov(std::vector<float> & xs, float avg_x, std::vector<float> & ys, float avg_y) {
  float sumxy = 0;
  for(std::size_t i = 0; i < xs.size(); i++) {
    sumxy += (xs[i] - avg_x) * (ys[i] - avg_y);
  }
  return sumxy / xs.size();
}

std::pair<float, float> least_square_method(std::vector<float> & xs, std::vector<float> & ys) {
  float avg_x = avg(xs);
  float avg_y = avg(ys);
  float sigma2_x = sigma2(xs, avg_x);
  float sigma2_y = sigma2(ys, avg_y);
  float cov_xy = cov(xs, avg_x, ys, avg_y);
  float a = cov_xy / sigma2_x;
  float b = avg_y - a * avg_x;
  return {a, b};
}


std::pair<float, float> solve_quadratic_equation(float a, float b, float c) {
  float d = b * b - 4 * a * c;
  if (d < 0) {
    return {0, 0};
  }
  return {(-b + sqrt(d)) / (2 * a), (-b - sqrt(d)) / (2 * a)};
}

std::tuple<float, float, float> pca_2d(std::vector<float> & xs, float avg_x,
                                       std::vector<float> & ys, float avg_y) {
  float xx = sigma2(xs, avg_x);
  float yy = sigma2(ys, avg_y);
  float xy = cov(xs, avg_x, ys, avg_y);
  auto [e1, e2] = solve_quadratic_equation(1.0, - xx - yy, xx * yy - xy * xy);
  float ey = 1.0;
  float ex = xy * ey / (e1 - xx);
  float a = sqrt(ex * ex + ey * ey);
  return {e1, ex / a, ey / a};
}
