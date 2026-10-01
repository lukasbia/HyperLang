add_library(HyperLLCVMBackend STATIC Backend.cpp)
target_include_directories(HyperLLCVMBackend PUBLIC \${PROJECT_SOURCE_DIR}/include)
target_link_libraries(HyperLLCVMBackend PUBLIC HyperLLCVM)
target_compile_features(HyperLLCVMBackend PUBLIC cxx_std_20)
