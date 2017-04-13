
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Qt include.
#include <QApplication>
#include <Qt3DExtras/Qt3DWindow>

// 3Dtree include.
#include "mainwindow.hpp"


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QScopedPointer< Qt3DExtras::Qt3DWindow > view( new Qt3DExtras::Qt3DWindow );

	MainWindow w( view );

	w.show();

	return app.exec();
}

