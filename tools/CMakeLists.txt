add_executable(hlc hlc/main.cpp)
target_link_libraries(hlc PRIVATE HyperLangCompiler)
target_compile_features(hlc PRIVATE cxx_std_20)
