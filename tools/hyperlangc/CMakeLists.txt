add_executable(hyperlangc main.cpp)

target_compile_features(hyperlangc PRIVATE cxx_std_20)
target_link_libraries(hyperlangc PRIVATE HyperLangParse)
target_include_directories(hyperlangc PRIVATE ${PROJECT_SOURCE_DIR}/include)
