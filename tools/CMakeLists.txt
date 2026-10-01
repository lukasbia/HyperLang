add_executable(hyperc hyperc.cpp)
target_include_directories(hyperc PRIVATE \${PROJECT_SOURCE_DIR}/include)
target_link_libraries(hyperc PRIVATE HyperSema HyperParse)
target_compile_features(hyperc PRIVATE cxx_std_20)
