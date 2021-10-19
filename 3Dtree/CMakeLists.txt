
project( 3Dtree )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6 COMPONENTS Widgets Core Gui 3DCore 3DRender 3DInput 3DExtras REQUIRED )

set( SRC main.cpp
	branch.cpp
	branch.hpp
	camera_controller.cpp
	camera_controller.hpp
	leaf.cpp
	leaf.hpp
	mainwindow.cpp
	mainwindow.hpp
	constants.hpp )

qt6_add_resources( SRC resources.qrc )

add_executable( 3Dtree ${SRC} )

target_link_libraries( 3Dtree Qt6::3DExtras Qt6::3DInput Qt6::3DRender
	Qt6::3DCore Qt6::Widgets Qt6::Gui Qt6::Core )
