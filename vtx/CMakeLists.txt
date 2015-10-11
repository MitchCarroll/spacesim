cmake_minimum_required(VERSION 2.6)
project(vtx)
add_library(${PROJECT_NAME} SHARED vtx.cpp)
#add_library(${PROJECT_NAME} STATIC vtx.cpp)
install(FILES vtx.h DESTINATION include)
install(TARGETS vtx
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)