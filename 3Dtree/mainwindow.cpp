
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
#include "camera_controller.hpp"

// Qt include.
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTimer>
#include <QFrame>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointLight>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QSkyboxEntity>
#include <Qt3DLogic/QFrameAction>


//! Grow timer in milliseconds.
static const int c_growTimer = 100;

//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	explicit MainWindowPrivate( MainWindow * parent )
		:	m_tree( Q_NULLPTR )
		,	m_growSpeed( 1.0f / 60.0f / ( 1000.0f / (float) c_growTimer ) )
		,	m_currentAge( 0.0f )
		,	m_startPos( 0.0f, -0.5f, 0.0f )
		,	m_endPos( 0.0f, 0.0f, 0.0f )
		,	m_age( 0 )
		,	m_years( Q_NULLPTR )
		,	m_btn( Q_NULLPTR )
		,	m_timer( Q_NULLPTR )
		,	m_secondTimer( Q_NULLPTR )
		,	m_playing( true )
		,	m_grown( false )
		,	m_rootEntity( Q_NULLPTR )
		,	m_lightEntity( Q_NULLPTR )
		,	m_branchMaterial( Q_NULLPTR )
		,	m_leafMesh( Q_NULLPTR )
		,	m_control( Q_NULLPTR )
		,	m_light( Q_NULLPTR )
		,	m_lightTransform( Q_NULLPTR )
		,	m_skyBox( Q_NULLPTR )
		,	m_entityCounterLabel( Q_NULLPTR )
		,	m_fpsLabel( Q_NULLPTR )
		,	m_entityCounter( 0 )
		,	m_fps( 1.0f )
		,	q( parent )
	{
	}

	~MainWindowPrivate()
	{
	}

	//! Init.
	void init( QScopedPointer< Qt3DExtras::Qt3DWindow > & view );
	//! Init 3D.
	void init3D( Qt3DExtras::Qt3DWindow * view );
	//! Create tree.
	void createTree();
	//! Delete tree.
	void deleteTree();

	//! Tree.
	Branch * m_tree;
	//! Grow speed.
	float m_growSpeed;
	//! Current age.
	float m_currentAge;
	//! Start tree pos.
	QVector3D m_startPos;
	//! End tree pos.
	QVector3D m_endPos;
	//! How old the tree?
	quint16 m_age;
	//! Years.
	QSpinBox * m_years;
	//! Pause/play button.
	QPushButton * m_btn;
	//! Timer.
	QTimer * m_timer;
	//! Second timer.
	QTimer * m_secondTimer;
	//! Playing?
	bool m_playing;
	//! Tree grown.
	bool m_grown;
	//! Root entity.
	Qt3DCore::QEntity * m_rootEntity;
	//! Light.
	Qt3DCore::QEntity * m_lightEntity;
	//! Branch material.
	Qt3DExtras::QPhongMaterial * m_branchMaterial;
	//! Leaf mesh.
	Qt3DRender::QMesh * m_leafMesh;
	//! Camera controller.
	CameraController * m_control;
	//! Light point.
	Qt3DRender::QPointLight * m_light;
	//! Light transform.
	Qt3DCore::QTransform * m_lightTransform;
	//! Sky box.
	Qt3DExtras::QSkyboxEntity * m_skyBox;
	//! Entity counter label.
	QLabel * m_entityCounterLabel;
	//! FPS label.
	QLabel * m_fpsLabel;
	//! Entity counter.
	quint64 m_entityCounter;
	//! FPS.
	float m_fps;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

void
MainWindowPrivate::init( QScopedPointer< Qt3DExtras::Qt3DWindow > & view )
{
	QWidget * container = QWidget::createWindowContainer( view.data(), q );

	Qt3DExtras::Qt3DWindow * window = view.take();

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
	m_years->setValue( 5 );
	l1->addWidget( m_years );

	m_btn = new QPushButton( MainWindow::tr( "Pause" ), q );
	v->addWidget( m_btn );

	QFrame * line = new QFrame( q );
	line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	v->addWidget( line );

	m_entityCounterLabel = new QLabel( q );
	m_entityCounterLabel->setText( MainWindow::tr( "Entities Count: %1" )
		.arg( m_entityCounter ) );
	v->addWidget( m_entityCounterLabel );

	m_fpsLabel = new QLabel( q );
	m_fpsLabel->setText( MainWindow::tr( "FPS: %1" ).arg( 0 ) );
	v->addWidget( m_fpsLabel );

	QSpacerItem * s = new QSpacerItem( 10, 10, QSizePolicy::Minimum,
		QSizePolicy::Expanding );

	v->addItem( s );

	m_timer = new QTimer( q );
	m_timer->setInterval( c_growTimer );
	m_timer->start();

	m_secondTimer = new QTimer( q );
	m_secondTimer->start( 1000 );

	MainWindow::connect( m_secondTimer, &QTimer::timeout,
		q, &MainWindow::second );
	MainWindow::connect( m_btn, &QPushButton::clicked,
		q, &MainWindow::buttonClicked );
	MainWindow::connect( m_timer, &QTimer::timeout,
		q, &MainWindow::timer );

	init3D( window );
}

void MainWindowPrivate::init3D( Qt3DExtras::Qt3DWindow * view )
{
	// Root entity
	QScopedPointer< Qt3DCore::QEntity > root( new Qt3DCore::QEntity );

	QScopedPointer< Qt3DLogic::QFrameAction > frameAction( new Qt3DLogic::QFrameAction );

	QObject::connect( frameAction.data(), &Qt3DLogic::QFrameAction::triggered,
		q, &MainWindow::frameProcessed );

	root->addComponent( frameAction.take() );

	m_branchMaterial = new Qt3DExtras::QPhongMaterial( root.data() );

	m_leafMesh = new Qt3DRender::QMesh( root.data() );
	m_leafMesh->setSource( QUrl( "qrc:/res/leaf.obj" ) );

	m_branchMaterial->setDiffuse( QColor( 41, 19, 0 ) );

	// Camera
	Qt3DRender::QCamera * cameraEntity = view->camera();

	cameraEntity->lens()->setPerspectiveProjection(
		45.0f, 16.0f / 9.0f, 0.1f, 1000.0f );
	cameraEntity->setPosition( QVector3D( 0.0f, 5.0f, 20.0f ) );
	cameraEntity->setUpVector( QVector3D( 0.0f, 1.0f, 0.0f ) );
	cameraEntity->setViewCenter( QVector3D( 0.0f, 5.0f, 0.0f ) );

	cameraEntity->rotateAboutViewCenter(
		Qt3DCore::QTransform::fromAxisAndAngle( 0.0f, 1.0f, 0.0f, 45.0f ) );

	m_lightEntity = new Qt3DCore::QEntity( root.data() );

	m_light = new Qt3DRender::QPointLight( m_lightEntity );
	m_light->setColor( Qt::white );
	m_light->setIntensity( 1.5f );
	m_lightEntity->addComponent( m_light );

	m_lightTransform = new Qt3DCore::QTransform(
		m_lightEntity );
	m_lightTransform->setTranslation( cameraEntity->position() );
	m_lightEntity->addComponent( m_lightTransform );

	m_control = new CameraController( cameraEntity, root.data() );

	m_skyBox = new Qt3DExtras::QSkyboxEntity( root.data() );
	m_skyBox->setBaseName( QStringLiteral( "qrc:/res/skybox" ) );
	m_skyBox->setExtension( QStringLiteral( ".tga" ) );

	const float baseScale = 0.1f;

	Qt3DCore::QTransform * skyTransform = new Qt3DCore::QTransform( m_skyBox );
	skyTransform->setTranslation( QVector3D( 0.0f, baseScale / 8.0f - 0.001f, 0.0f ) );
	skyTransform->setScale3D( QVector3D( baseScale, baseScale / 4.0f, baseScale ) );
	m_skyBox->addComponent( skyTransform );

	m_rootEntity = root.data();

	createTree();

	view->setRootEntity( root.take() );
}

void
MainWindowPrivate::createTree()
{
	deleteTree();

	if( c_useInstancedRendering )
		m_leafMesh->setInstanceCount( 0 );

	m_age = 0;

	m_tree = new Branch( m_startPos, m_endPos, m_age,
		c_startBranchRadius,
		true, true,
		m_branchMaterial, m_leafMesh,
		m_timer, Q_NULLPTR, m_entityCounter, m_rootEntity, true );

	m_tree->setAge( 0.0f );
	m_tree->updatePosition();
	m_tree->placeLeafs();
}

void
MainWindowPrivate::deleteTree()
{
	if( m_tree )
	{
		m_tree->deleteLater();

		m_tree = Q_NULLPTR;
	}
}


//
// MainWindow
//

MainWindow::MainWindow( QScopedPointer< Qt3DExtras::Qt3DWindow > & view )
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
	if( d->m_grown )
	{
		d->m_playing = true;

		d->m_grown = false;

		d->m_btn->setText( tr( "Pause" ) );

		d->m_currentAge = 0.0f;

		d->createTree();

		d->m_timer->start();
	}
	else if( d->m_playing )
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
MainWindow::timer()
{
	d->m_currentAge += d->m_growSpeed;

	if( d->m_currentAge > (float) d->m_years->value() - 0.5f )
	{
		d->m_timer->stop();

		d->m_grown = true;

		d->m_btn->setText( tr( "Restart" ) );

		d->m_playing = false;
	}
	else if( d->m_tree )
		d->m_tree->setAge( d->m_currentAge );

	d->m_entityCounterLabel->setText( MainWindow::tr( "Entities Count: %1" )
		.arg( d->m_entityCounter ) );
}

void
MainWindow::frameProcessed( float )
{
	d->m_fps += 1.0f;
}

void
MainWindow::second()
{
	d->m_fpsLabel->setText( MainWindow::tr( "FPS: %1" ).arg( qRound( d->m_fps ) ) );

	d->m_fps = 0.0f;
}
