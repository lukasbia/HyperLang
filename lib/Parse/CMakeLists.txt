set(HYPER_PARSE_SOURCES
    Confusables.cpp
    Lexer.cpp
    ParseDecl.cpp
    ParseDeclName.cpp
    ParseExpr.cpp
    ParseGeneric.cpp
    ParseIfConfig.cpp
    ParsePattern.cpp
    ParseRegex.cpp
    ParseRequests.cpp
    ParseStmt.cpp
    ParseType.cpp
    ParseVersion.cpp
    Parser.cpp
    PersistentParserState.cpp
)

add_library(HyperParse STATIC
    ${HYPER_PARSE_SOURCES}
)

target_include_directories(HyperParse
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperParse
    PUBLIC
        cxx_std_20
)
