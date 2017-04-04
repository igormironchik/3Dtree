
TEMPLATE = app

CONFIG += c++14

QT += widgets core gui \
    3dcore 3drender 3dinput 3dextras

HEADERS = branch.hpp \
	leaf.hpp \
	mainwindow.hpp

SOURCES = branch.cpp \
	leaf.hpp \
	main.cpp \
	mainwindow.cpp
