
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
#include "leaf.hpp"
#include "constants.hpp"

// Qt include.
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMesh>

#include <QtMath>
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>
#include <QTimer>

// C++ include.
#include <cmath>
#include <random>


using namespace Qt3DExtras;


//
// LeafPrivate
//

class LeafPrivate {
public:
	LeafPrivate( const QVector3D & startBranchPos,
		const QVector3D & endBranchPos, Qt3DRender::QMesh * mesh,
		QTimer * animationTimer, Leaf * parent )
		:	m_mesh( mesh )
		,	m_material( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_startBranchPos( &startBranchPos )
		,	m_endBranchPos( &endBranchPos )
		,	m_timer( animationTimer )
		,	m_fallAngle( 0.0f )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Mesh.
	Qt3DRender::QMesh * m_mesh;
	//! Material.
	QScopedPointer< QPhongMaterial > m_material;
	//! Transform.
	QScopedPointer< Qt3DCore::QTransform > m_transform;
	//! Start branch position.
	const QVector3D * m_startBranchPos;
	//! End branch position.
	const QVector3D * m_endBranchPos;
	//! Timer.
	QTimer * m_timer;
	//! Fall vector.
	QVector3D m_fallVectorStartPos;
	//! Fall vector.
	QVector3D m_fallVectorEndPos;
	//! Fall rotate angle.
	float m_fallAngle;
	//! Parent.
	Leaf * q;
}; // class LeafPrivate

void
LeafPrivate::init()
{
	q->addComponent( m_mesh );

	m_material.reset( new QPhongMaterial );

	m_material->setDiffuse( Qt::darkGreen );

	q->addComponent( m_material.data() );

	m_transform.reset( new Qt3DCore::QTransform );

	m_transform->setTranslation( *m_endBranchPos );

	q->addComponent( m_transform.data() );
}


//
// Leaf
//

Leaf::Leaf( const QVector3D & startBranchPos,
	const QVector3D & endBranchPos, Qt3DRender::QMesh * mesh,
	QTimer * animationTimer,
	Qt3DCore::QNode * parent )
	:	Qt3DCore::QEntity( parent )
	,	d( new LeafPrivate( startBranchPos, endBranchPos, mesh,
			animationTimer, this ) )
{
	d->init();
}

Leaf::~Leaf()
{
}

void
Leaf::setColor( const QColor & c )
{
	d->m_material->setDiffuse( c );
}

QColor
Leaf::autumnColor()
{
	static QImage img;

	static std::random_device rd;
	static std::mt19937 gen( rd() );
	static std::uniform_int_distribution< int > autumn( 0, 99 );

	if( img.isNull() )
	{
		QPixmap pixmap( 100, 2 );

		QPainter p( &pixmap );

		QLinearGradient g( 0.0, 1.0, 100.0, 1.0 );
		g.setColorAt( 0.0, Qt::yellow );
		g.setColorAt( 1.0, Qt::red );

		p.setBrush( g );

		p.drawRect( 0, 0, 100, 2 );

		img = pixmap.toImage();
	}

	return img.pixelColor( autumn( gen ), 1 );
}

void
Leaf::setAge( float age )
{
	if( age > 1.0f )
		age = 1.0f;
	else if( age < 0.0f )
		age = 0.0f;

	d->m_transform->setScale( c_leafBaseScale * age );
}

void
Leaf::updatePosition()
{
	d->m_transform->setTranslation( *( d->m_endBranchPos ) );
}

void
Leaf::rotate( float angle )
{
	const QVector3D branch = ( *( d->m_endBranchPos ) -
		*( d->m_startBranchPos ) ).normalized();
	const QVector3D leaf( 0.0f, 1.0f, 0.0f );

	QVector3D axis = QVector3D::crossProduct( branch, leaf );

	if( axis.isNull() )
		axis = QVector3D(  1.0f, 0.0f, 0.0f );

	const float cosPlainAngle = QVector3D::dotProduct( branch, leaf );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, c_leafAngle );

	const float plainAngle = qRadiansToDegrees( std::acos( cosPlainAngle ) ) +
		dis( gen );

	const QQuaternion quat = Qt3DCore::QTransform::fromAxesAndAngles(
		axis, plainAngle, branch, angle );

	d->m_transform->setRotation( quat );
}

void
Leaf::fallAndDie()
{
	d->m_fallVectorEndPos = *( d->m_endBranchPos );

	d->m_fallVectorStartPos = *( d->m_endBranchPos ) - QVector3D( 0.0f, 0.5f, 0.0f );

	d->m_startBranchPos = &( d->m_fallVectorStartPos );

	d->m_endBranchPos = &( d->m_fallVectorEndPos );

	connect( d->m_timer, &QTimer::timeout,
		this, &Leaf::timeout );
}

void
Leaf::timeout()
{
	if( d->m_endBranchPos->y() <= 0.0f )
		deleteLater();
	else
	{
		rotate( d->m_fallAngle );

		d->m_fallVectorEndPos -= QVector3D( 0.0f, 0.05f, 0.0f );

		d->m_fallVectorStartPos = d->m_fallVectorEndPos -
			QVector3D( 0.0f, 0.5f, 0.0f );

		d->m_fallAngle += 15.0f;

		updatePosition();
	}
}
