add_executable(hyperc hyperc/main.cpp)
target_link_libraries(hyperc PRIVATE HyperCompiler)
target_compile_features(hyperc PRIVATE cxx_std_20)
