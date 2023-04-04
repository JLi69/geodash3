#Minimum version of cmake required to compile
cmake_minimum_required(VERSION 3.11)

#Name of project
project(geodash3-editor)

include_directories(${CMAKE_SOURCE_DIR}/../glad/include)

find_package(OpenGL REQUIRED)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_FLAGS "-mwindows -O2 -static-libstdc++ -static-libgcc -DWINDOWS  -static -lpthread")
	set(APP_ICON_RESOURCE_WINDOWS ${CMAKE_SOURCE_DIR}/geodash3.rc) 
else()
	set(CMAKE_CXX_FLAGS "-O2 -static-libstdc++ -static-libgcc")
	set(APP_ICON_RESOURCE_WINDOWS "") 
endif()

#Create the executable
add_executable(
	${PROJECT_NAME} #Name of executable

	#Files to compile
	${PROJECT_SOURCE_DIR}/Main.cpp
	#${PROJECT_SOURCE_DIR}/glad.c

	#OpenGL utilities
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Shader.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/VertexBufferObj.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/GetOpenGLError.cpp
	#Textures
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Texture-Utils/TextureCoords.cpp
	${PROJECT_SOURCE_DIR}/src/GL-Utils/Texture-Utils/TextureObj.cpp

	#Button
	${PROJECT_SOURCE_DIR}/src/Button/Button.cpp

	#init
	${PROJECT_SOURCE_DIR}/src/Init/init.cpp
	
	#open file
	${PROJECT_SOURCE_DIR}/src/File/OpenFile.cpp

	#Engine
	${PROJECT_SOURCE_DIR}/src/Engine.cpp
	${PROJECT_SOURCE_DIR}/src/EngineDisplay.cpp
	${PROJECT_SOURCE_DIR}/src/EngineKeyInput.cpp
	${PROJECT_SOURCE_DIR}/src/EngineUpdate.cpp
	${PROJECT_SOURCE_DIR}/src/EngineHandleMouseInput.cpp
	${PROJECT_SOURCE_DIR}/src/EngineHandleScrollInput.cpp	
	${PROJECT_SOURCE_DIR}/src/EngineUpdate.cpp
	${PROJECT_SOURCE_DIR}/src/EngineSaveFile.cpp	
	${PROJECT_SOURCE_DIR}/src/EngineExport.cpp
	${PROJECT_SOURCE_DIR}/src/EngineOnWinResize.cpp
	${PROJECT_SOURCE_DIR}/src/EngineMenu.cpp

	#Gameobjects
	${PROJECT_SOURCE_DIR}/src/GameObject/Block.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/Spike.cpp
	${PROJECT_SOURCE_DIR}/src/GameObject/Collider.cpp

	#Level loading
	${PROJECT_SOURCE_DIR}/src/Level/level.cpp

	${PROJECT_SOURCE_DIR}/../glad/glad.c

	${APP_ICON_RESOURCE_WINDOWS}
	)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(${PROJECT_NAME} gdi32)
	target_link_libraries(${PROJECT_NAME} SOIL2)	
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3)
else()
	target_link_libraries(${PROJECT_NAME} SOIL2)	
	target_link_libraries(${PROJECT_NAME} OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} glfw3) 
endif()
