# CMakeLists.txt for project POLYMNIA

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_VERBOSE_MAKEFILE 1)

#PROJECT(polymnia CXX)

SET(CMAKE_CXX_FLAGS "-Wall -std=c++17 -O3")
#SET(CMAKE_CXX_FLAGS "-Wall -std=c++14 -O3")
#SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O3")
include_directories("${CMAKE_INSTALL_PREFIX}/include")

ADD_LIBRARY(polymnia STATIC
  ibuf.cpp
  picture.cpp
  tograyscale24.cpp
  to_gs_indexed.cpp
  magnify.cpp
  reduce.cpp
  toindexed.cpp
  dibin.cpp dibout.cpp
  jpegin.cpp jpegout.cpp jpegio.cpp
  pngin.cpp pngout.cpp
)

INSTALL(TARGETS polymnia DESTINATION lib)
INSTALL(FILES
    ibuf.h ibuf-imp_.h
    rgb.h
    picture.h
    pictcvt.h
    pictio.h
    dibio.h
    jpegio.h
    pngio.h
  DESTINATION
    include/polymnia
)


#eof
