cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 OLD)
list(APPEND CMAKE_MODULE_PATH "modules")

##### BUILD ############################################################

project(sampler)

set(EXECUTABLE_OUTPUT_PATH ./bin)

find_package(Boost 1.42.0 COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGl REQUIRED)
find_package(OpenGL REQUIRED)
Find_Package (SDL REQUIRED )
Find_Package (SDL_image REQUIRED )

include(${QT_USE_FILE})

include_directories(
	${PROJECT_SOURCE_DIR}/src
	${Boost_INCLUDE_DIR}
	${QT_QTOPENGL_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
)

link_directories(
	${Boost_LIBRARY_DIRS}
	${OPENGL_LIBRARY_DIRS}
)

file(GLOB sampler-core-src ${PROJECT_SOURCE_DIR}/src/*.cpp )
file(GLOB sampler-gui-src ${PROJECT_SOURCE_DIR}/src/gui/*.cpp )
file(GLOB sampler-gui-hdr ${PROJECT_SOURCE_DIR}/src/gui/*.hpp )

add_executable(sampler-cli ${PROJECT_SOURCE_DIR}/src/main/cli.cpp ${sampler-core-src})
target_link_libraries(
	sampler-cli
	${Boost_LIBRARIES}
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
)

qt4_wrap_cpp(sampler-gui-moc ${sampler-gui-hdr})
add_executable(sampler-gui src/main/gui.cpp ${sampler-gui-moc} ${sampler-gui-src} ${sampler-core-src})
target_link_libraries(
	sampler-gui
	${Boost_LIBRARIES}
	${QT_LIBRARIES}
	${OPENGL_LIBRARY}
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
)

add_executable(sampler-test src/main/test.cpp ${sampler-core-src})
target_link_libraries(
	sampler-test
	${Boost_LIBRARIES}
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
)

install(
	PROGRAMS
	./bin/sampler-cli
	./bin/sampler-gui
	DESTINATION bin
)
