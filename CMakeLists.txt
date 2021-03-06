# Copyright 2015 by Matthijs van Dorp.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6)
project(CHATHRAN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CHATHRAN_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_net REQUIRED)
find_package(Assimp REQUIRED)
find_package(Lua REQUIRED)
find_package(Selene REQUIRED)
find_package(Tinygame REQUIRED)
find_package(ChathranGame REQUIRED)

configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wextra -Wshadow -ansi -pedantic -std=c++11 -ldl") # ldl for lua loadlib.c which uses it; c++11 for Selene
#set(CMAKE_CXX_FLAGS "-O3 -Wall -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "-lrt")
#set(CMAKE_VERBOSE_MAKEFILE true)

#add_subdirectory(${TINY_SOURCE_DIR}/tiny/)

include_directories(${OPENGL_INCLUDE_DIR})
link_directories(${OPENGL_LIBRARY_DIR})
include_directories(${GLEW_INCLUDE_DIR})
link_directories(${GLEW_LIBRARY_DIR})
include_directories(${OPENAL_INCLUDE_DIR})
include_directories(${OGGVORBIS_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
link_directories(${ASSIMP_LIBRARY_DIR})
include_directories(${SDL_INCLUDE_DIR})
link_directories(${SDL_LIBRARY_DIR})
include_directories(${SDL_IMAGE_INCLUDE_DIR})
link_directories(${SDL_IMAGE_LIBRARY_DIR})
include_directories(${SDL_TTF_INCLUDE_DIR})
link_directories(${SDL_TTF_LIBRARY_DIR})
include_directories(${SDL_NET_INCLUDE_DIR})
link_directories(${SDL_NET_LIBRARY_DIR})
include_directories(${LUA_INCLUDE_DIR})
link_directories(${LUA_LIBRARY_DIR})
include_directories(${TINYGAME_INCLUDE_DIR})
link_directories(${TINYGAME_LIBRARY_DIR})
include_directories(${SELENE_INCLUDE_DIR})
include_directories(${CHATHRAN_SOURCE_DIR})
include_directories(${CHATHRAN_BINARY_DIR})
include_directories(${CHATHRAN_SOURCE_DIR}/src/)

set(USED_LIBS 
              ${TINYGAME_LIBRARIES} # MUST be first, otherwise tinygame's calls to the below libraries don't get linked!
              ${LUA_LIBRARY}
              ${ASSIMP_LIBRARIES}
              ${OPENGL_LIBRARY}
              ${GLEW_LIBRARY}
              ${OPENAL_LIBRARY}
              ${OGGVORBIS_LIBRARIES}
              ${SDL_LIBRARY}
              ${SDLIMAGE_LIBRARY}
              ${SDLTTF_LIBRARY}
              ${SDLNET_LIBRARY})

file(GLOB CHATHRAN_SOURCES RELATIVE ${CHATHRAN_SOURCE_DIR}
	src/core/*.cpp
	src/core/interface/*.cpp
	src/veg/*.cpp
)

add_executable(test_Font src/test_Font.cpp)
target_link_libraries(test_Font ${USED_LIBS})

add_executable(chathran src/chathran.cpp ${CHATHRAN_SOURCES})
target_link_libraries(chathran ${USED_LIBS})

