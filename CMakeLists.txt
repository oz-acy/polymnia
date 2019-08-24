# CMakeLists.txt for project POLYMNIA

cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE 1)
project(polymnia CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)



# 依存するライブラリ
find_package(themis REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Boost REQUIRED)


# ソースファイルとヘッダファイル
set(POLYMNIA_SOURCES
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

set(POLYMNIA_HEADERS
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



# Release版ライブラリ
add_library(polymnia STATIC ${POLYMNIA_SOURCES})

set_target_properties(polymnia
  PROPERTIES PUBLIC_HEADER "${POLYMNIA_HEADERS}"
)

target_include_directories(polymnia PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(polymnia themis png jpeg z)

target_compile_options(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

target_compile_definitions(polymnia PRIVATE NDEBUG)

target_link_options(polymnia PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static>
  $<$<CXX_COMPILER_ID:GNU>:-static>
)


# Debug版ライブラリ
add_library(polymnia-debug STATIC ${POLYMNIA_SOURCES})

target_include_directories(polymnia-debug PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(polymnia-debug themis png jpeg z)

target_compile_options(polymnia-debug PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -g3 -ggdb>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -g3 -ggdb>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_options(polymnia-debug PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static>
  $<$<CXX_COMPILER_ID:GNU>:-static>
)


# Resizer (Releaseのみ)
add_executable(resizer
  resizer_main.cpp
  resizer_loadsave.cpp
)

target_compile_options(resizer PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

target_link_options(resizer PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static>
  $<$<CXX_COMPILER_ID:GNU>:-static>
)
target_compile_definitions(resizer PRIVATE NDEBUG)
target_link_libraries(resizer polymnia themis)




# インストールの設定

install(TARGETS polymnia
  EXPORT polymnia-export
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/polymnia
)

install(EXPORT polymnia-export
  FILE polymnia-config.cmake
  DESTINATION share/cmake/polymnia/
#  EXPORT_LINK_INTERFACE_LIBRARIES
)

install(TARGETS polymnia-debug
  ARCHIVE DESTINATION lib
)

install(TARGETS resizer
  DESTINATION bin
)




#eof
