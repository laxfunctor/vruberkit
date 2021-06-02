#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <variant>

#include "statistics.hpp"
#include "threedim.hpp"

using namespace cv;

const td::screen scr = {2, {0,0,0}, {5,0,0}, {0,5,0}};
const std::vector<td::colored_triangle> fixed_objs = { };
const std::pair<td::point, std::vector<td::colored_triangle>> face =
  {
   // center of face
   {2.5, 1, -2},
   {
    // main face
    {td::EMIT,
     {0.94, 0.84, 0.7},
     {{0, 0, -1}, {5, 0, -1}, {2.5, 5, -1}}},
    // hair
    {td::EMIT,
     {0.1, 0.1, 0.5},
     {{2.5 - 1, 3, -0.9}, {2.5 + 1, 3, -0.9}, {2.5, 5, -0.9}}},
    // eyes
    {td::EMIT,
     {0, 0, 0},
     {{2 - 0.3, 2, -0.9}, {2 + 0.3, 2, -0.9}, {2, 2.3, -0.9}}},
    {td::EMIT,
     {0, 0, 0},
     {{3 - 0.3, 2, -0.9}, {3 + 0.3, 2, -0.9}, {3, 2.3, -0.9}}},
    // mouse
    {td::EMIT,
     {1, 0.2, 0.2},
     {{2, 1, -0.9}, {3, 1, -0.9}, {2.5, 0.7, -0.9}}},
   }
  };

Mat calc(const td::screen & scr,
         const std::vector<td::colored_triangle> & fixed_objs,
         const std::pair<td::point, std::vector<td::colored_triangle>> & face,
         const double theta,
         const double phi) {
  const unsigned int scale = 8;
  const unsigned int h = 50;
  const unsigned int w = 50;
  Mat ret = Mat::zeros(scale * h, scale * w, CV_8UC3);
  std::vector<td::colored_triangle> objs = fixed_objs;
  const auto & [face_center, face_objs] = face;
  for (const auto & obj : face_objs) {
    objs.push_back(rotate_y(phi, rotate_z(theta, obj, face_center), face_center));
  }
  auto img = td::shoot(scr, h, w, objs);
  for (std::size_t i = 0; i < h; i++) {
    for (std::size_t j = 0; j < w; j++) {
      if (std::holds_alternative<td::Collide>(img[i][j])) {
        auto [r, g, b] = std::get<td::Collide>(img[i][j]).c;
        for (std::size_t di = 0; di < scale; di++) {
          for (std::size_t dj = 0; dj < scale; dj++) {
            ret.at<Vec3b>(scale * (h - i) - di - 1, scale * j + dj) =
              {
               (unsigned char)(255 * b),
               (unsigned char)(255 * g),
               (unsigned char)(255 * r)
              };
          }
        }
      } else {
        for (std::size_t di = 0; di < scale; di++) {
          for (std::size_t dj = 0; dj < scale; dj++) {
            ret.at<Vec3b>(scale * (h - i) - di - 1, scale * j + dj) = {0, 255, 0};
          }
        }

         ret.at<Vec3b>(h - 1 - i, j) = {0, 255, 0};
      }
    }
  }
  return ret;
}


int main(int, char**) {
  VideoCapture cap(0);
  if(!cap.isOpened()) return -1;

  Mat frame;

  int width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
  int height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
  double fps = cap.get(CAP_PROP_FPS);
  
  std::cout << "fps=" << fps << " width=" << width << " height=" << height << std::endl;

  std::string frame_window_name = "";
  namedWindow(frame_window_name, WINDOW_AUTOSIZE);

  for (;;) {
    cap >> frame;
    flip(frame, frame, 1);
    
    std::vector<float> hada_xs, hada_ys;

    for (int x = 5; x < width - 5; x++) {
      for (int y = 5; y < height - 5; y++) {
        const auto & pixel = frame.at<Vec3b>(y, x);
        float r = pixel[2], g = pixel[1], b = pixel[0];
        bool pushed = false;
        if (g * 1.1 <= r && r <= g * 1.5
            && b * 1.1 <= r && r <= b * 1.4
            && r >= 100.0) {
          if (!pushed) {
            hada_xs.push_back(x);
            hada_ys.push_back(y);
            pushed = true;
          }
        }
        if (g * 1.1 <= r && r <= g * 1.3
            && b * 1.2 <= r && r <= b * 1.4
            && r >= 70.0) {
          if (!pushed) {
            hada_xs.push_back(x);
            hada_ys.push_back(y);
            pushed = true;
          }
        }
        if (g * 1.1 <= r && r <= g * 1.2
            && b * 1.2 <= r && r <= b * 1.3
            && r >= 70.0) {
          if (!pushed) {
            hada_xs.push_back(x);
            hada_ys.push_back(y);
            pushed = true;
          }
        }
        if (g * 1.2 <= r && r <= g * 1.5
            && b * 1.2 <= r && r <= b * 1.4
            && r >= 80.0) {
          if (!pushed) {
            hada_xs.push_back(x);
            hada_ys.push_back(y);
            pushed = true;
          }
        }
        /* for demonstration
        if (pushed) {
          frame.at<Vec3b>(y, x) = {241, 187, 147};
        } else {
          frame.at<Vec3b>(y, x) = {0, 0, 0};
        }
        */
      }
    }

    // shisei
    float avg_x = avg(hada_xs);
    float avg_y = avg(hada_ys);
    float sigma2_x = sigma2(hada_xs, avg_x);
    float sigma_x = std::sqrt(sigma2_x);
    float sigma2_y = sigma2(hada_ys, avg_y);
    float sigma_y = std::sqrt(sigma2_y);
    auto [e, ex, ey] = pca_2d(hada_xs, avg_x, hada_ys, avg_y);
    auto [a, b] = least_square_method(hada_xs, hada_ys);
    const double theta = std::atan2(ex, ey);
    const double phi = 0;

    /* render */
    Mat virtualworld = calc(scr, fixed_objs, face, theta, phi);

    /* draw */
    imshow(frame_window_name, virtualworld);

    /* for demonstration
    int n = 10;
    cv::resize(frame, frame, cv::Size(), 1.0 / n, 1.0 / n);
    cv::resize(frame, frame, cv::Size(), n, n, cv::INTER_NEAREST);
    line(frame,
         Point((int)(avg_x - ex * sigma_y * 2), (int)(avg_y - ey * sigma_y * 2)),
         Point((int)(avg_x + ex * sigma_y), (int)(avg_y + ey * sigma_y)),
         Scalar(0,255,255), 5, 8, 0);
    imshow("frame", frame);
    */

    if(waitKey(1) == 27) break;
  }
  return 0;
}
