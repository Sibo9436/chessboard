cmake_minimum_required(VERSION 3.20)

# Evito di cazzare e buildare i test e gli esempi
	set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(glfw)
add_subdirectory(glm)
#include_directories(imgui/backends)
	set (IMGUI_SOURCES 
			imgui/imconfig.h imgui/imgui.h imgui/imgui_internal.h imgui/imstb_rectpack.h
			imgui/imstb_textedit.h imgui/imstb_truetype.h imgui/imgui.cpp imgui/imgui_demo.cpp
			imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
	    )
	set (GLAD_SOURCES
			glad/include/glad/glad.h
			glad/include/KHR/khrplatform.h
			glad/src/glad.c
	    )
  set (STB_SOURCES 
      stb/stb_image.h 
      stb/stb_impl.cpp
      )

#Creazione della lib di dearImGui
add_library(imgui STATIC ${IMGUI_SOURCES})

add_library(imgui_backend STATIC 
			imgui/backends/imgui_impl_glfw.h
			imgui/backends/imgui_impl_glfw.cpp
			imgui/backends/imgui_impl_opengl3.h
			imgui/backends/imgui_impl_opengl3.cpp
			)
target_include_directories(imgui_backend PUBLIC imgui)
target_link_libraries(imgui_backend  imgui glfw)
#Creazione della lib GLAD
add_library(glad STATIC ${GLAD_SOURCES})
target_include_directories(glad PUBLIC glad/include)
#Creazione della lib di STB
add_library(stb STATIC ${STB_SOURCES})
target_include_directories(stb PUBLIC stb)
