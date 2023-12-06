cmake_minimum_required(VERSION 3.1.15)
project(BuildDll)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
    set(platform_sources
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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/handle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4aplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/k4atypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/K4aLoader.h
    global.c
    K4aLoader.cpp
)

add_library(k4aLoader SHARED
    ${common_sources}
    ${platform_sources}
)

if(WIN32)
    target_link_libraries(k4aLoader
        kernel32
    )
else()
    target_link_libraries(k4aLoader
        pthread
        dl
    )
endif()

install(TARGETS k4aLoader DESTINATION ${CMAKE_BINARY_DIR}/install/lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/K4aLoader.h DESTINATION ${CMAKE_BINARY_DIR}/install/include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/k4atypes.h DESTINATION ${CMAKE_BINARY_DIR}/install/include)