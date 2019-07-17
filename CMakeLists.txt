# CMakeLists.txt for project POLYMNIA

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_VERBOSE_MAKEFILE 1)
PROJECT(polymnia CXX)

ENABLE_LANGUAGE(CXX)
SET(CMAKE_CXX_STANDARD 17)


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

SET(POLYMNIA_HEADERS
  ibuf.h
  ibuf-imp_.h
  rgb.h
  picture.h
  pictio.h
  dibio.h
  jpegio.h
  pngio.h
  hex.h
)

SET_TARGET_PROPERTIES(polymnia
  PROPERTIES PUBLIC_HEADER "${POLYMNIA_HEADERS}"
)


TARGET_COMPILE_OPTIONS(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

TARGET_LINK_OPTIONS(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static>
  $<$<CXX_COMPILER_ID:GNU>:-static>
)

TARGET_INCLUDE_DIRECTORIES(polymnia PRIVATE ${CMAKE_INSTALL_PREFIX}/include)


INSTALL(TARGETS polymnia
  EXPORT polymnia-export
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/polymnia
)

INSTALL(EXPORT polymnia-export
  FILE themis-config.cmake
  DESTINATION share/cmake/themis/
#  EXPORT_LINK_INTERFACE_LIBRARIES
)


#eof
