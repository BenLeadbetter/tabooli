find_package(Catch2 REQUIRED)

set(TABOOLI_TEST_SOURCE
    main.cpp
    ostream_operators.cpp
    visitor.cpp
)

add_executable(tabooli-tests ${TABOOLI_TEST_SOURCE})
target_link_libraries(tabooli-tests PRIVATE Catch2::Catch2 tabooli)
tabooli_set_target_properties(tabooli-tests)