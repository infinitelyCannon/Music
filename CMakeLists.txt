cmake_minimum_required(VERSION 3.1.0)
prokect(MusicApp)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(extern/Mx3)

set(SOURCES src/main.cpp
	src/MainWindow.cpp
	src/Player.cpp
	src/TrackItem.cpp
)

set(Q_OBJS src/MainWindow.h
	src/Player.h
	src/TrackItem.h
)

set(UI src/ui/MainWindow.ui)

qt5_wrap_ui(ui_wrap ${UI})
qt5_wrap_cpp(moc_sources ${Q_OBJS})

add_excutable(MusicApp ${SOUCES} ${ui_wrap} ${moc_sources})
target_compile_features(MusicApp PRIVATE cxx_std_14)
target_include_directories(MusicApp PRIVATE "${CMAKE_SOURCE_DIR/src/ui")
target_link_libraries(MusicApp Qt5::Core Qt5::Gui Qt5::Widgets Mx3::Mx3)