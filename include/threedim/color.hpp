#ifndef THREEDIM_COLOR
#define THREEDIM_COLOR

namespace td {
  typedef struct {
    float red, green, blue;
  } color;

  const color white = {1,1,1};
  const color black = {0,0,0};
  const color red = {1,0,0};
  const color green = {0,1,0};
  const color blue = {0,0,1};

  color scale_color(const color & a, const color & c);
}

#endif
