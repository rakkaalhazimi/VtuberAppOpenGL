cmake_minimum_required (VERSION 3.10)

# Setup projectname
project(VtuberApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Executable filename
set(EXE_FILE "app")

# Enable static linking of libstdc++ (std::filesystem won't work without this)
# if(MINGW)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
# endif()

# Include Folders
# include_directories(include include/imgui)


# Source Folders
file(GLOB_RECURSE SOURCES 
	"src/*.cpp"
	"src/*.c"
	"src/*.h"
	"external/*.cpp"
)

# Compile and link main.cpp and foo.cpp
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    include
    include/imgui
)

# Link Libraries
target_link_libraries(${PROJECT_NAME} 
  freetype
  glfw3
  opengl32
  gdi32 # windows GUI
  winmm # windows multimedia: timers, audio and input devices
)

# Post-build: move (copy) the .exe to /bin/
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
      "${CMAKE_SOURCE_DIR}/bin"
    
    COMMAND ${CMAKE_COMMAND} -E copy
      $<TARGET_FILE:${PROJECT_NAME}>
      "${CMAKE_SOURCE_DIR}/bin/${EXE_FILE}.exe"
    COMMENT "Moving final exe to bin/ folder"
    
    COMMAND ${CMAKE_COMMAND} -E remove
      $<TARGET_FILE:${PROJECT_NAME}>  # Deletes the original .exe
    COMMENT "Removing the original exe after copying"
)

# Copy the dll on the same level of exe file
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#       "${CMAKE_SOURCE_DIR}/libs/glfw/glfw3.dll"
#       "${CMAKE_SOURCE_DIR}/bin/glfw3.dll"
# )
