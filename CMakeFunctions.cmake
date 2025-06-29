function(add_static_library LIB_NAME)
    add_library(${LIB_NAME} STATIC ${ARGN})

    # Create library-specific include directory in BUILD directory
    set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_NAME}")
    file(MAKE_DIRECTORY ${LIB_INCLUDE_DIR})

    file(GLOB HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
    file(GLOB IPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.ipp")
    set(ALL_HEADER_FILES ${HPP_FILES} ${IPP_FILES})

    foreach(HEADER ${ALL_HEADER_FILES})
        get_filename_component(HEADER_NAME ${HEADER} NAME)
        get_filename_component(HEADER_ABS ${HEADER} ABSOLUTE)
    
        file(CREATE_LINK 
            ${HEADER_ABS}
            ${LIB_INCLUDE_DIR}/${HEADER_NAME}
            SYMBOLIC
        )
    endforeach()

    target_include_directories(${LIB_NAME}
        PUBLIC
            "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
endfunction()

function(depend_and_link TARGET_NAME)
    set(LINK_MODE PUBLIC)  
    set(LIBRARIES_TO_LINK)
    
    foreach(arg IN LISTS ARGN)
        if(arg MATCHES "^(PUBLIC|PRIVATE|INTERFACE)$")
            set(LINK_MODE ${arg})
        else()
            list(APPEND LIBRARIES_TO_LINK ${arg})
        endif()
    endforeach()

    if(LIBRARIES_TO_LINK)
        target_link_libraries(${TARGET_NAME}
            ${LINK_MODE}
                ${LIBRARIES_TO_LINK}
        )
    endif()
endfunction()

function(add_test_executable TEST_NAME)
    set(SOURCES)
    foreach(arg IN LISTS ARGN)
        list(APPEND SOURCES ${arg})
    endforeach()
    
    add_executable(${TEST_NAME} ${SOURCES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
    
    # Add the test target to the global list
    set(ALL_TEST_TARGETS ${ALL_TEST_TARGETS} ${TEST_NAME} CACHE INTERNAL "List of all test targets")
endfunction()
