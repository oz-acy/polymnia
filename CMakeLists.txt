# CMakeLists.txt for project POLYMNIA

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
SET(CMAKE_VERBOSE_MAKEFILE 1)
PROJECT(polymnia CXX)

ENABLE_LANGUAGE(CXX)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

SET(Boost_USE_STATIC_LIBS ON)


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

FIND_PACKAGE(themis REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
TARGET_INCLUDE_DIRECTORIES(polymnia PUBLIC ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(polymnia themis png jpeg)

TARGET_COMPILE_OPTIONS(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

TARGET_LINK_OPTIONS(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static>
  $<$<CXX_COMPILER_ID:GNU>:-static>
)


INSTALL(TARGETS polymnia
  EXPORT polymnia-export
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/polymnia
)

INSTALL(EXPORT polymnia-export
  FILE polymnia-config.cmake
  DESTINATION share/cmake/polymnia/
#  EXPORT_LINK_INTERFACE_LIBRARIES
)


#eof
