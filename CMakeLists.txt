cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 OLD)
list(APPEND CMAKE_MODULE_PATH "cmake")

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
        ${PROJECT_BINARY_DIR}
)

link_directories(
	${Boost_LIBRARY_DIRS}
	${OPENGL_LIBRARY_DIRS}
)



######## External Data for production/barycenter/lut files######
##
## Data files will be located in the {build-folder}/data/lut/
## 
include(ExternalData) 
set (ExternalData_TIMEOUT_ABSOLUTE 0)
list(APPEND ExternalData_URL_TEMPLATES
  "http://liris.cnrs.fr/polyhex/data/%(algo)-%(hash)")
ExternalData_Expand_Arguments(
  GetData
  OUT-data 
  DATA{data/lut/production_rules.dat}
  DATA{data/lut/barycenters.dat}
  )
MESSAGE(STATUS "PATH to the data files: ${OUT-data}")
ExternalData_Add_Target(GetData)
ExternalData_Expand_Arguments(
  GetLUT
  OUT-LUT 
  DATA{data/lut/offsets_bnot.dat}
  )
MESSAGE(STATUS "PATH to the LUT file: ${OUT-LUT}")
ExternalData_Add_Target(GetLUT)
set_target_properties(GetLUT PROPERTIES EXCLUDE_FROM_ALL "TRUE")
##########"


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

#------------------------------------------------------------------------------
# Configuration of the Config.h
#------------------------------------------------------------------------------
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_BINARY_DIR}/config.h)
