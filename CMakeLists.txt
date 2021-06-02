cmake_minimum_required(VERSION 3.1)
project(vtuber_kit CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# statistics
add_library(statistics
  STATIC
  "./lib/statistics/statistics.cpp"
  )

# threedim
add_library(threedim
  STATIC
  "./lib/threedim/color.cpp"
  "./lib/threedim/geometry.cpp"
  "./lib/threedim/camera.cpp"
  )
target_include_directories(threedim
  PRIVATE "./include/threedim"
  )

# vrun
add_executable(vrun "./src/vrun.cpp")
target_include_directories(vrun
  PUBLIC "/usr/local/include/opencv4"
  PUBLIC "./include/statistics" "./include/threedim"
  )
target_link_libraries(vrun
  PUBLIC opencv_core opencv_imgproc opencv_imgcodecs opencv_videoio opencv_highgui
  PUBLIC statistics threedim
  )
