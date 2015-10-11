cmake_minimum_required(VERSION 2.6)
project(quat)
add_library(${PROJECT_NAME} SHARED quat.cpp)
#add_library(${PROJECT_NAME} STATIC quat.cpp)
target_link_libraries(vtx)
install(FILES quat.h DESTINATION include)
install(TARGETS quat
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
