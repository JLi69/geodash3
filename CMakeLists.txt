#Minimum version of cmake required to compile
cmake_minimum_required(VERSION 3.11)

#Name of project
project(geodash3)

add_subdirectory(openal-soft EXCLUDE_FROM_ALL)
add_subdirectory(libsndfile EXCLUDE_FROM_ALL)

include_directories(${CMAKE_SOURCE_DIR}/gllibs/include)
include_directories(${CMAKE_SOURCE_DIR}/glm)
include_directories(${CMAKE_SOURCE_DIR}/openal-soft/include)
include_directories(${CMAKE_SOURCE_DIR}/libsndfile/include)

link_directories(${CMAKE_BINARY_DIR})
link_directories(${CMAKE_BINARY_DIR}/libsndfile)
link_directories(${CMAKE_BINARY_DIR}/openal-soft)
find_package(OpenGL REQUIRED)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_FLAGS "-DWINDOWS -O2 -static-libstdc++ -static-libgcc -mwindows -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
	add_custom_target(
		libsndfile
		COMMAND cd libsndfile && mingw32-make sndfile
		)
	add_custom_target(
		openal-soft
		COMMAND cd openal-soft && mingw32-make OpenAL	
		)
else()
	set(CMAKE_CXX_FLAGS "-Wl,-rpath=${ORIGIN} -O2 -flto -static-libstdc++ -static-libgcc")
	add_custom_target(
		libsndfile
		COMMAND cd libsndfile && make sndfile
		)
	add_custom_target(
		openal-soft
		COMMAND cd openal-soft && make OpenAL	
		)
endif()

#Create the executable
add_executable(
	${PROJECT_NAME} #Name of executable

	#Files to compile
	${PROJECT_SOURCE_DIR}/Main.cpp

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

	${PROJECT_SOURCE_DIR}/gllibs/lib/glad.c
)

add_dependencies(${PROJECT_NAME} libsndfile openal-soft)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(${PROJECT_NAME} gdi32 pthread)
	target_link_libraries(${PROJECT_NAME} SOIL2)	
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3)
	target_link_libraries(${PROJECT_NAME} OpenAL32)
	target_link_libraries(${PROJECT_NAME} sndfile)	
else()
	target_link_libraries(${PROJECT_NAME} SOIL2)
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3)
	target_link_libraries(${PROJECT_NAME} openal)
	target_link_libraries(${PROJECT_NAME} sndfile)
endif()
