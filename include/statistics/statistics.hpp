#ifndef VT_STATISTICS
#define VT_STATISTICS
#include <vector>

float avg(std::vector<float> & v);
float sigma2(std::vector<float> & v, float avg);
float cov(std::vector<float> & xs, float avg_x, std::vector<float> & ys, float avg_y);
std::pair<float, float> least_square_method(std::vector<float> & xs, std::vector<float> & ys);
std::pair<float, float> solve_quadratic_equation(float a, float b, float c);
std::tuple<float, float, float> pca_2d(std::vector<float> & xs, float avg_x,
                                       std::vector<float> & ys, float avg_y);

#endif
