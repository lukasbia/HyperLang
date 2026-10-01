add_library(HyperSema STATIC Sema.cpp)
target_include_directories(HyperSema PUBLIC \${PROJECT_SOURCE_DIR}/include)
target_link_libraries(HyperSema PUBLIC HyperParse)
target_compile_features(HyperSema PUBLIC cxx_std_20)
