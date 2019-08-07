cmake_minimum_required(VERSION 3.1.0)
project(MusicApp)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(vendor/Mx3)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES src/main.cpp
	src/MainWindow.cpp
	src/Player.cpp
	src/TrackItem.cpp)

qt5_wrap_ui(SOURCES src/ui/MainWindow.ui)

add_executable(MusicApp ${SOURCES} res/resources.qrc)
target_compile_features(MusicApp PRIVATE cxx_std_14)
target_include_directories(MusicApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/ui")
target_link_libraries(MusicApp Qt5::Core Qt5::Gui Qt5::Widgets Mx3)

if(WIN32)
add_custom_command(TARGET MusicApp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	"${CMAKE_CURRENT_BINARY_DIR}/vendor/Mx3/Mx3.dll"
	"${CMAKE_CURRENT_BINARY_DIR}"
)
endif()
