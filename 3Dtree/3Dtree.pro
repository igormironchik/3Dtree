
TEMPLATE = app

CONFIG += c++14

QT += widgets core gui \
    3dcore 3drender 3dinput 3dextras

HEADERS = branch.hpp \
	leaf.hpp \
	mainwindow.hpp \
    leaf_geometry.hpp \
    leaf_mesh.hpp

SOURCES = branch.cpp \
	leaf.hpp \
	main.cpp \
	mainwindow.cpp \
    leaf_geometry.cpp \
    leaf_mesh.cpp
