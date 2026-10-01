add_library(HyperAST STATIC AST.cpp)
target_include_directories(HyperAST PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperAST PUBLIC cxx_std_20)
