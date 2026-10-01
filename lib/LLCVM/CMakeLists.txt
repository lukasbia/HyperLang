add_library(HyperLLCVM STATIC LLCVM.cpp)
target_include_directories(HyperLLCVM PUBLIC \${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperLLCVM PUBLIC cxx_std_20)
