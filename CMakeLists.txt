# CMake entry point
cmake_minimum_required (VERSION 3.11)
project (OpenGL_Practice)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system) 

pkg_check_modules(GLFW3 REQUIRED glfw3)

if(OPENGL_FOUND AND GLEW_FOUND AND PKG_CONFIG_FOUND AND GLFW3_FOUND AND Boost_FOUND)

    message("GLFW3 LIB: ${GLFW3_LIBRARIES}")
    message("GLEW LIB: ${GLEW_LIBRARIES}")
    message("OPENGL LIB: ${OPENGL_LIBRARIES}")
    message("Boost LIB: ${Boost_LIBRARIES}")

    include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
    )
    
    add_executable(moving_triangle 
        MovingTriangle/moving_triangle.cpp
    )

    add_executable(toggle_cube 
        ToggleCube/toggle_cube.cpp
    )

    #target_link_libraries(first_window -lglfw -lGL -lX11
    #-lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread)

    target_link_libraries(moving_triangle
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${CMAKE_DL_LIBS}
        -lX11
        -pthread
    )

    target_link_libraries(toggle_cube
        ${Boost_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${CMAKE_DL_LIBS}
        -lX11
        -pthread
    )

    add_custom_command( TARGET moving_triangle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/MovingTriangle/*.glsl
                ${CMAKE_CURRENT_BINARY_DIR})
    
    add_custom_command( TARGET toggle_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/ToggleCube/*.glsl
                ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command( TARGET toggle_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/ToggleCube/*.bmp
                ${CMAKE_CURRENT_BINARY_DIR})

endif()
