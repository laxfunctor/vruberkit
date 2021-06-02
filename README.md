# VTuber kit

Use Ubuntu 20.04.

## Install OpenCV

Download the source code `opencv-4.x.x.zip`
from https://opencv.org/releases/ .

```
$ sudo apt install cmake libgtk2.0-dev pkg-config
$ unzip opencv-4.5.2.zip
$ cd opencv-4.5.2
$ mkdir build
$ cd ./build
$ cmake ..
$ cmake --build .
$ make -j4
$ sudo make install
$ sudo ldconfig
```

## Building

```
$ mkdir build
$ cd ./build
$ cmake ..
$ make
```
