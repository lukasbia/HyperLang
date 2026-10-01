add_library(HyperHIL STATIC HIL.cpp HILBuilder.cpp)
target_include_directories(HyperHIL PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperHIL PUBLIC cxx_std_20)
