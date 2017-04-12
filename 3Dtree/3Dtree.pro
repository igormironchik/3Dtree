
TEMPLATE = app

CONFIG += c++14

QT += widgets core gui \
    3dcore 3drender 3dinput 3dextras

HEADERS = branch.hpp \
	leaf.hpp \
	mainwindow.hpp \
    leaf_geometry.hpp \
    leaf_mesh.hpp \
    constants.hpp \
	camera_controller.hpp

SOURCES = branch.cpp \
	leaf.cpp \
	main.cpp \
	mainwindow.cpp \
    leaf_geometry.cpp \
    leaf_mesh.cpp \
	camera_controller.cpp
