cmake_minimum_required(VERSION 3.22)
include(FetchContent)

project(game
	VERSION 1.0.0
	DESCRIPTION "Game with Box2D x raylib integration"
	HOMEPAGE_URL "https://recurse.com"
	LANGUAGES C CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(PC "Build on PC" OFF)

if (PC)
   FetchContent_Declare(
		box2d
		GIT_REPOSITORY https://github.com/erincatto/box2d.git
		GIT_TAG main
		GIT_SHALLOW TRUE
		GIT_PROGRESS TRUE
		)
	FetchContent_MakeAvailable(box2d)

	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG master
		GIT_SHALLOW TRUE
		GIT_PROGRESS TRUE
	)
	FetchContent_MakeAvailable(raylib)
else()
	add_compile_options(-DSENSOR)
endif()

add_library(i2c_bno055 i2c_bno055.c)
add_executable(game game.cpp)

if (PC)
	target_link_libraries(game PRIVATE box2d raylib i2c i2c_bno055)
else()
	target_link_libraries(game PRIVATE box2d raylib m GL drm EGL gbm i2c_bno055)
endif()
