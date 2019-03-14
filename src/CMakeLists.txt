add_library(lib bmp-handler.cc)
add_executable(src main.cc)
target_link_libraries(src lib)
