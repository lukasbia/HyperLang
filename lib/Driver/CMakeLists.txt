add_library(HyperLangDriver STATIC
  Action.cpp
  Compilation.cpp
  DarwinToolChains.cpp
  Driver.cpp
  FrontendUtil.cpp
  Job.cpp
  PrettyStackTrace.cpp
  ToolChain.cpp
  ToolChains.cpp
  UnixToolChains.cpp
  WebAssemblyToolChains.cpp
  WindowsToolChains.cpp
)

target_include_directories(HyperLangDriver
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(HyperLangDriver PUBLIC cxx_std_20)
