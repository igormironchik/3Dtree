
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

// 3Dtree include.
#include "mainwindow.hpp"
#include "branch.hpp"
#include "constants.hpp"

// Qt include.
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTimer>
#include <QApplication>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DCore/QTransform>


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_tree( Q_NULLPTR )
		,	m_startPos( 0.0f, -0.5f, 0.0f )
		,	m_endPos( 0.0f, 0.0f, 0.0f )
		,	m_years( Q_NULLPTR )
		,	m_btn( Q_NULLPTR )
		,	m_timer( Q_NULLPTR )
		,	m_playing( true )
		,	q( parent )
	{
	}

	//! Init.
	void init( Qt3DExtras::Qt3DWindow * view );
	//! Init 3D.
	void init3D( Qt3DExtras::Qt3DWindow * view );

	//! Tree.
	Branch * m_tree;
	//! Start tree pos.
	QVector3D m_startPos;
	//! End tree pos.
	QVector3D m_endPos;
	//! Years.
	QSpinBox * m_years;
	//! Pause/play button.
	QPushButton * m_btn;
	//! Timer.
	QTimer * m_timer;
	//! Playing?
	bool m_playing;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

void
MainWindowPrivate::init( Qt3DExtras::Qt3DWindow * view )
{
	QWidget * container = QWidget::createWindowContainer( view, q );
	container->setMinimumSize( QSize( 768, 600 ) );
	container->setMaximumSize( QSize( 768, 600 ) );

	QHBoxLayout * l = new QHBoxLayout( q );
	l->addWidget( container );

	QVBoxLayout * v = new QVBoxLayout;
	l->addLayout( v );

	QHBoxLayout * l1 = new QHBoxLayout;
	v->addLayout( l1 );

	QLabel * label = new QLabel( MainWindow::tr( "Maximum Years" ), q );
	l1->addWidget( label );

	m_years = new QSpinBox( q );
	m_years->setMinimum( 1 );
	m_years->setMaximum( 99 );
	m_years->setValue( 30 );
	l1->addWidget( m_years );

	m_btn = new QPushButton( MainWindow::tr( "Pause" ), q );
	v->addWidget( m_btn );

	QSpacerItem * s = new QSpacerItem( 10, 10, QSizePolicy::Minimum,
		QSizePolicy::Expanding );

	v->addItem( s );

	m_timer = new QTimer( q );
	m_timer->setInterval( 1000 );
	m_timer->start();

	void ( QSpinBox::*signal )( int ) = &QSpinBox::valueChanged;

	MainWindow::connect( m_years, signal,
		q, &MainWindow::yearsChanged );
	MainWindow::connect( m_btn, &QPushButton::clicked,
		q, &MainWindow::buttonClicked );
	MainWindow::connect( m_timer, &QTimer::timeout,
		q, &MainWindow::timer );

	init3D( view );
}

static Qt3DCore::QEntity * rootEntity = Q_NULLPTR;

static void cleanRootEntity()
{
	delete rootEntity;
}

void MainWindowPrivate::init3D( Qt3DExtras::Qt3DWindow * view )
{
	qAddPostRoutine( cleanRootEntity );

	// Root entity
	rootEntity = new Qt3DCore::QEntity;

	// Camera
	Qt3DRender::QCamera * cameraEntity = view->camera();

	cameraEntity->lens()->setPerspectiveProjection(
		45.0f, 16.0f / 9.0f, 0.1f, 1000.0f );
	cameraEntity->setPosition( QVector3D( 0.0f, 0.0f, 20.0f ) );
	cameraEntity->setUpVector( QVector3D( 0.0f, 1.0f, 0.0f ) );
	cameraEntity->setViewCenter( QVector3D( 0.0f, 0.0f, 0.0f ) );

	Qt3DCore::QEntity * lightEntity = new Qt3DCore::QEntity( rootEntity );

	Qt3DRender::QPointLight * light = new Qt3DRender::QPointLight( lightEntity );
	light->setColor( Qt::white );
	light->setIntensity( 1.0f );
	lightEntity->addComponent( light );

	Qt3DCore::QTransform * lightTransform = new Qt3DCore::QTransform(
		lightEntity );
	lightTransform->setTranslation( cameraEntity->position() );
	lightEntity->addComponent( lightTransform );

	m_tree = new Branch( m_startPos, m_endPos, c_startBranchRadius,
		true, rootEntity );
}


//
// MainWindow
//

MainWindow::MainWindow( Qt3DExtras::Qt3DWindow * view )
	:	d( new MainWindowPrivate( this ) )
{
	d->init( view );
}

MainWindow::~MainWindow()
{
}

void
MainWindow::buttonClicked()
{
	if( d->m_playing )
	{
		d->m_playing = false;

		d->m_btn->setText( tr( "Play" ) );

		d->m_timer->stop();
	}
	else
	{
		d->m_playing = true;

		d->m_btn->setText( tr( "Pause" ) );

		d->m_timer->start();
	}
}

void
MainWindow::yearsChanged( int years )
{

}

void
MainWindow::timer()
{

}
