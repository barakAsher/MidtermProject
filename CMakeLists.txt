cmake_minimum_required(VERSION 3.14)
project(mp)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp mpEngine.cpp mpEngine.h characters/Skiff.cpp characters/Skiff.h
        Player.h
        characters/Vehicle.h
        characters/Vehicle.cpp
        characters/Starlord.h
        characters/Starlord.cpp
        characters/Gengiben.h
        characters/Gengiben.cpp
        FirstPersonCam.cpp
        FirstPersonCam.h
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Windows with MinGW Installations
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" AND MINGW )
    # if working on Windows but not in the lab
    # update the include directory location
    include_directories("C:/openGL/include")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "C:/openGL/lib")
    target_link_libraries(${PROJECT_NAME} md5model opengl32 glfw3 gdi32 glew32.dll)
    # OS X Installations
# OS X Installations
elseif( APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    # update the include directory location
    include_directories("/usr/local/include")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/lib")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" glfw3 glew)
# Blanket *nix Installations
elseif( UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    # update the include directory location
    include_directories("/usr/local/include" "/usr/include")
    # update the lib directory location
    target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/lib" "/usr/lib")
    # update the linked libraries
    target_link_libraries(${PROJECT_NAME} GL glfw GLEW)
endif()