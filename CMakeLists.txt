#Minimum version of cmake required to compile
cmake_minimum_required(VERSION 3.10)

#Name of project
project(geodash3)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	include_directories(${CMAKE_SOURCE_DIR}/dependencies/sndfile-win)
else()
	include_directories(${CMAKE_SOURCE_DIR}/dependencies/sndfile-linux)
	link_directories(${CMAKE_BINARY_DIR})
endif()

find_package(OpenGL REQUIRED)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_FLAGS "-DWINDOWS -O2 -static-libstdc++ -static-libgcc -mwindows -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")	
else()
	set(CMAKE_CXX_FLAGS "-Wl,-rpath=${ORIGIN} -O2 -flto -static-libstdc++ -static-libgcc")		
endif()

#Create the executable
add_executable(
	${PROJECT_NAME} #Name of executable

	#Files to compile
	${PROJECT_SOURCE_DIR}/Main.cpp
	${PROJECT_SOURCE_DIR}/glad.c

	#OpenGL utilities
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Shader.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/VertexBufferObj.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/GetOpenGLError.cpp
	#Textures
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Texture-Utils/TextureCoords.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Texture-Utils/TextureObj.cpp

	#audio
	${PROJECT_SOURCE_DIR}/src/Audio-Utils/SoundDevice.cpp
	${PROJECT_SOURCE_DIR}/src/Audio-Utils/SoundBuffer.cpp
	${PROJECT_SOURCE_DIR}/src/Audio-Utils/SoundSource.cpp
	${PROJECT_SOURCE_DIR}/src/Audio-Utils/MusicBuffer.cpp

	#init
	${PROJECT_SOURCE_DIR}/src/Init/init.cpp

	#Button
	${PROJECT_SOURCE_DIR}/src/Button/Button.cpp

	#open files
	${PROJECT_SOURCE_DIR}/src/File/OpenFile.cpp

	#Engine
	${PROJECT_SOURCE_DIR}/src/Engine.cpp
	${PROJECT_SOURCE_DIR}/src/EngineDisplay.cpp
	${PROJECT_SOURCE_DIR}/src/EngineKeyInput.cpp
	${PROJECT_SOURCE_DIR}/src/EngineUpdate.cpp
	${PROJECT_SOURCE_DIR}/src/EngineRun.cpp
	${PROJECT_SOURCE_DIR}/src/EngineHandleMouseInput.cpp
	${PROJECT_SOURCE_DIR}/src/EngineOnWinResize.cpp

	#Gameobjects
	${PROJECT_SOURCE_DIR}/src/GameObject/Player.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/Block.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/Spike.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/Collider.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/DeathParticle.cpp

	#Level loading
	${PROJECT_SOURCE_DIR}/src/Level/level.cpp
)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(${PROJECT_NAME} gdi32 pthread)
	#target_link_libraries(${PROJECT_NAME} user32 kernel32 winspool.lib
	#						comdlg32 advapi32 shell32 ole32 oleaut32
	#						uuid odbc32 odbccp32 opengl32)		
	target_link_libraries(${PROJECT_NAME} SOIL)	
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3)
	target_link_libraries(${PROJECT_NAME} OpenAL32)
	target_link_libraries(${PROJECT_NAME} sndfile)	
else()
	target_link_libraries(${PROJECT_NAME} soil)
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3)
	target_link_libraries(${PROJECT_NAME} openal)
	target_link_libraries(${PROJECT_NAME} sndfile)
endif()

#find_library(glfw3lib glfw3 PATHS ${CMAKE_SOURCE_DIR}/dependencies/lib-vc2019)
#if(${glfw3lib} STREQUAL glfw3lib-NOTFOUND)
#	message(FATAL "glfw not found!")
#else()
#	message("glfw found at " ${glfw3lib})
#	target_link_libraries(${PROJECT_NAME} ${glfw3lib})
#endif()
