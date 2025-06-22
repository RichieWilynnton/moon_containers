function(add_static_library LIB_NAME)
    add_library(${LIB_NAME} STATIC ${ARGN})
    
    # Create library-specific include directory
    set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB_NAME}")
    file(MAKE_DIRECTORY ${LIB_INCLUDE_DIR})
    
    # Symlink/copy headers (platform independent) - now includes both .hpp and .ipp files
    file(GLOB HEADER_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.ipp"
    )
    foreach(HEADER ${HEADER_FILES})
        get_filename_component(HEADER_NAME ${HEADER} NAME)
        configure_file(
            ${HEADER}
            ${LIB_INCLUDE_DIR}/${HEADER_NAME}
            COPYONLY
        )
    endforeach()
    
    # Set include paths
    target_include_directories(${LIB_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endfunction()
