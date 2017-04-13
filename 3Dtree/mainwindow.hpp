
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

#ifndef TREE__MAINWINDOW_HPP__INCLUDED
#define TREE__MAINWINDOW_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

namespace Qt3DExtras {
	class Qt3DWindow;
}

QT_END_NAMESPACE


//
// MainWindow
//

class MainWindowPrivate;

//! Main window.
class MainWindow Q_DECL_FINAL
	:	public QWidget
{
public:
	explicit MainWindow( QScopedPointer< Qt3DExtras::Qt3DWindow > & view );
	~MainWindow();

private slots:
	//! Play/pause button clicked.
	void buttonClicked();
	//! Timer.
	void timer();

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow


#endif // TREE__MAINWINDOW_HPP__INCLUDED
