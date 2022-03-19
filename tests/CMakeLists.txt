find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)

set(TABOOLI_TEST_SOURCE
    main.cpp
    test_cases/binary_tree.cpp
    test_cases/expression.cpp
    test_cases/boolean.cpp
    utility/test_data.cpp
)

add_executable(tabooli-tests ${TABOOLI_TEST_SOURCE})
target_link_libraries(tabooli-tests PRIVATE Catch2::Catch2 tabooli)
target_include_directories(tabooli-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${Boost_INCLUDE_DIRECTORIES})
tabooli_set_target_properties(tabooli-tests)