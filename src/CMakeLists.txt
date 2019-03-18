add_library(lib BmpImage.cc OctTree.cc)
add_executable(src main.cc)
target_link_libraries(src lib)
