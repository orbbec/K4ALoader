cmake_minimum_required(VERSION 3.1.15)
project(k4aLoader)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME}
    main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
    set(platform_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/include/k4a_export.h
        dynlib_windows.c
    )
else()
    set(platform_sources
        dynlib_linux.c
    )
endif()

set(common_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dynlib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/handle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4atypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4a.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4aloader/k4a.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4aLoader.h
    k4aLoader.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
    ${common_sources}
    ${platform_sources}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        kernel32
    )
else()
    target_link_libraries(${PROJECT_NAME}
        pthread
        dl
    )
endif()