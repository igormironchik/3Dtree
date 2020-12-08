
TEMPLATE = app

CONFIG += c++14

QT += widgets core gui \
    3dcore 3drender 3dinput 3dextras 3dlogic

RESOURCES = resources.qrc

HEADERS = branch.hpp \
	leaf.hpp \
	mainwindow.hpp \
    constants.hpp \
	camera_controller.hpp

SOURCES = branch.cpp \
	leaf.cpp \
	main.cpp \
	mainwindow.cpp \
	camera_controller.cpp
