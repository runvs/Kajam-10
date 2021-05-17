cmake_minimum_required (VERSION 3.10)

#####################################################
## Variables to be set depending on project
#####################################################
set (PROJECTNAME "Kajam10")

# Apple users: set to /usr/local/Cellar/sfml/2.4.2_1/lib/ or respectively
if (WIN32)
	set (SFML_DIR_ROOT "${CMAKE_SOURCE_DIR}/ext/SFML-2.5.1/" CACHE PATH "Path to SFML root dir.")
endif()

#####################################################
## Helper Functions
#####################################################
function (jt_use_assets TGT)
	add_custom_command(TARGET ${TGT} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
		
	if(MSVC)
		add_custom_command(TARGET ${TGT} PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Debug/assets)
		add_custom_command(TARGET ${TGT} PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Release/assets)
	endif()
endfunction()

function (jt_deploy_dlls DESTFOLDER)
	message (STATUS "Deploy to ${DESTFOLDER}")

	if (WIN32)
		file(MAKE_DIRECTORY ${DESTFOLDER})
		# copy dlls

		if ($ENV{CLION_IDE})
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-2.dll ${DESTFOLDER} COPYONLY)

			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${CMAKE_SOURCE_DIR}/ext/tileson-master/external_libs/source/sfml/extlibs/bin/x64/openal32.dll ${DESTFOLDER} COPYONLY)
		elseif(MSVC)
			file(MAKE_DIRECTORY ${DESTFOLDER}/Debug)
			file(MAKE_DIRECTORY ${DESTFOLDER}/Release)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-2.dll ${DESTFOLDER} COPYONLY)
			
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-2.dll ${DESTFOLDER}/Release COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-2.dll ${DESTFOLDER}/Release COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-2.dll ${DESTFOLDER}/Release COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-2.dll ${DESTFOLDER}/Release COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-2.dll ${DESTFOLDER}/Release COPYONLY)

			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-d-2.dll ${DESTFOLDER} COPYONLY)
			
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-d-2.dll ${DESTFOLDER}/Debug COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-d-2.dll ${DESTFOLDER}/Debug COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-d-2.dll ${DESTFOLDER}/Debug COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-d-2.dll ${DESTFOLDER}/Debug COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-network-d-2.dll ${DESTFOLDER}/Debug COPYONLY)
			
			configure_file(${CMAKE_SOURCE_DIR}/ext/tileson-master/external_libs/source/sfml/extlibs/bin/x64/openal32.dll ${DESTFOLDER} COPYONLY)
			configure_file(${CMAKE_SOURCE_DIR}/ext/tileson-master/external_libs/source/sfml/extlibs/bin/x64/openal32.dll ${DESTFOLDER}/Debug COPYONLY)
			configure_file(${CMAKE_SOURCE_DIR}/ext/tileson-master/external_libs/source/sfml/extlibs/bin/x64/openal32.dll ${DESTFOLDER}/Release COPYONLY)

		endif()
	endif()
endfunction()

if (WIN32)
	set (SFML_DIR "${SFML_DIR_ROOT}/lib/cmake/SFML")
#	find_package(SFML COMPONENTS graphics window system audio network REQUIRED)
endif()
#####################################################
## Other variables and compiler setup
#####################################################
set (CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD 17)
add_compile_definitions(DISABLE_CPP17_FILESYSTEM)

if(WIN32)
	## nasty warning 5205 from tileson should not show up,
	## so the warning level of this one is set to 4 and the global warning level is set to three
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w45205 /W3 /Ox /EHsc")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 --std=c++17 -fpermissive -lstdc++fs")
endif()

#####################################################
## Create Project file and link directories
#####################################################
project (${PROJECTNAME})

if (WIN32 OR APPLE)
	link_directories(${SFML_DIR_ROOT}/lib)
endif()

#####################################################
## Setup Box2d
#####################################################
file(GLOB_RECURSE BOX2DFILES ext/box2d/Box2D/*)
add_library(Box2D ${BOX2DFILES})
target_include_directories(Box2D SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/box2d)

#####################################################
## setup JamTemplate
#####################################################

file(GLOB_RECURSE JAMTEMPLATEBASEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/JamTemplate/common/*)
file(GLOB_RECURSE JAMTEMPLATEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/JamTemplate/sfml/*)

add_library(JamTemplate ${JAMTEMPLATEFILES} ${JAMTEMPLATEBASEFILES})

# add public includes, so they can be used by dependent targets
target_include_directories(JamTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/JamTemplate/common)
target_include_directories(JamTemplate SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/tileson-master/include)

target_include_directories(JamTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/JamTemplate/sfml)
target_include_directories(JamTemplate SYSTEM PUBLIC ${SFML_DIR_ROOT}/include)


# group together similar files for a cleaner JamTemplate Project in IDEs that support this feature
source_group(
	GameLoop FILES
	src/JamTemplate/common/GameInterface.hpp

	src/JamTemplate/common/GameLoopInterface.hpp

	src/JamTemplate/common/GameBase.cpp
	src/JamTemplate/common/GameBase.hpp

	src/JamTemplate/common/GameObject.cpp
	src/JamTemplate/common/GameObject.hpp

	src/JamTemplate/common/GameState.cpp
	src/JamTemplate/common/GameState.hpp

	src/JamTemplate/sfml/Game.cpp
	src/JamTemplate/sfml/Game.hpp
)
source_group(
	Drawables FILES
	src/JamTemplate/common/Animation.cpp
	src/JamTemplate/common/Animation.hpp

	src/JamTemplate/common/Bar.cpp
	src/JamTemplate/common/Bar.hpp

	src/JamTemplate/common/DrawableImpl.cpp
	src/JamTemplate/common/DrawableImpl.hpp

	src/JamTemplate/common/DrawableInterface.hpp

	src/JamTemplate/common/DrawableHelpers.cpp
	src/JamTemplate/common/DrawableHelpers.hpp

	src/JamTemplate/sfml/Shape.cpp
	src/JamTemplate/sfml/Shape.hpp

	src/JamTemplate/sfml/Sprite.cpp
	src/JamTemplate/sfml/Sprite.hpp

	src/JamTemplate/sfml/Text.cpp
	src/JamTemplate/sfml/Text.hpp

	src/JamTemplate/common/Tilemap.hpp
	src/JamTemplate/common/Tilemap.cpp
)
source_group(Tweens REGULAR_EXPRESSION src/JamTemplate/common/Tween*)

target_link_libraries(JamTemplate optimized sfml-system )
target_link_libraries(JamTemplate optimized sfml-window )
target_link_libraries(JamTemplate optimized sfml-graphics )
target_link_libraries(JamTemplate optimized sfml-audio )
target_link_libraries(JamTemplate optimized sfml-network )

if (WIN32)
	target_link_libraries(JamTemplate debug sfml-system-d )
	target_link_libraries(JamTemplate debug sfml-window-d )
	target_link_libraries(JamTemplate debug sfml-graphics-d )
	target_link_libraries(JamTemplate debug sfml-audio-d )
	target_link_libraries(JamTemplate debug sfml-network-d )
endif()

target_link_libraries(JamTemplate Box2D)

#####################################################
## setup Game executable
#####################################################

file(GLOB COMMONFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Common/*)
add_library(${PROJECTNAME}_Common ${COMMONFILES})
target_include_directories(${PROJECTNAME}_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Common)
target_link_libraries(${PROJECTNAME}_Common JamTemplate)

file(GLOB CLIENTFILES src/Game/Client/*)
add_executable(${PROJECTNAME}_Client  ${CLIENTFILES})
target_link_libraries(${PROJECTNAME}_Client ${PROJECTNAME}_Common)

file(GLOB SERVERFILES src/Game/Server/*)
add_executable(${PROJECTNAME}_Server  ${SERVERFILES})
target_link_libraries(${PROJECTNAME}_Server ${PROJECTNAME}_Common)

jt_use_assets(${PROJECTNAME}_Client)
jt_use_assets(${PROJECTNAME}_Server)

#####################################################
## copy sfml dlls and .clang-format around
#####################################################

jt_deploy_dlls(${CMAKE_BINARY_DIR})
if(MSVC)
	jt_deploy_dlls(${CMAKE_BINARY_DIR})
endif()
configure_file(${CMAKE_SOURCE_DIR}/.clang-format ${CMAKE_BINARY_DIR}/ COPYONLY)

##################################################
## copy config.json to Debug/Release binary dir ##
##################################################

if(MSVC)
	configure_file(${CMAKE_SOURCE_DIR}/src/config.json ${CMAKE_BINARY_DIR}/Debug   COPYONLY)
	configure_file(${CMAKE_SOURCE_DIR}/src/config.json ${CMAKE_BINARY_DIR}/Release COPYONLY)
	configure_file(${CMAKE_SOURCE_DIR}/src/config.json ${CMAKE_BINARY_DIR} COPYONLY)
endif()