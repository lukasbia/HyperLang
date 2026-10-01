add_executable(hyperlang-lex hyperlang-lex/main.cpp)
target_link_libraries(hyperlang-lex PRIVATE hyper_parse)
target_compile_features(hyperlang-lex PRIVATE cxx_std_20)
