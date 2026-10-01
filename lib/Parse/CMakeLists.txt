add_library(HyperParse STATIC Lexer.cpp Parser.cpp)
target_include_directories(HyperParse PUBLIC \${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperParse PUBLIC cxx_std_20)
