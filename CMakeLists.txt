cmake_minimum_required(VERSION 3.28.3)

project(hmfocx)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(hmfocx STATIC ${SRC_LIST})

target_include_directories(hmfocx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)





# main switch
option(HMFOCX_BUILD_TEST "Build test programs"  OFF)

set(
CONTAINERS 
"LIST" "MAP" "POOL" "STACK" "QUEUE" "HEAP" "SET" "ARR" "STR" 
)

# create test option
foreach(CONTAINER IN LISTS CONTAINERS)
    option(HMFOCX_${CONTAINER}_TEST "TEST FOR ${CONTAINER}" OFF)
endforeach()


if (HMFOCX_BUILD_TEST)
    enable_testing()

    # run test when variable is true
    foreach(CONTAINER IN LISTS CONTAINERS)
        message(STATUS "HMFOCX_${CONTAINER}_TEST = ${HMFOCX_${CONTAINER}_TEST}")        # output message
        if (HMFOCX_${CONTAINER}_TEST)
            string(TOLOWER "${CONTAINER}" container)
            add_subdirectory("test/${container}")
        endif()
    endforeach()
endif()