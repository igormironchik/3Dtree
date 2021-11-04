
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
#include "branch.hpp"

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
#include <memory>


using namespace Qt3DExtras;


//
// LeafPrivate
//

class LeafPrivate {
public:
	LeafPrivate( const QVector3D & startBranchPos,
		const QVector3D & endBranchPos, Qt3DRender::QMesh * mesh,
		QTimer * animationTimer, Branch * parentBranch, Leaf * parent,
		quint64 & entityCounter, bool useInstanceRendering )
		:	m_mesh( mesh )
		,	m_material( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_startBranchPos( &startBranchPos )
		,	m_endBranchPos( &endBranchPos )
		,	m_timer( animationTimer )
		,	m_fallAngle( 0.0f )
		,	m_branch( parentBranch )
		,	q( parent )
		,	m_entityCounter( entityCounter )
		,	m_useInstanceRendering( useInstanceRendering )
		,	m_startBranchRot( 0.0f )
		,	m_leafDistRot( -1.0f )
		,	m_fallAndDie( false )
	{
		++m_entityCounter;
	}

	~LeafPrivate()
	{
		--m_entityCounter;
	}

	//! Init.
	void init();

	//! Mesh.
	Qt3DRender::QMesh * m_mesh;
	//! Material.
	QPhongMaterial * m_material;
	//! Transform.
	Qt3DCore::QTransform * m_transform;
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
	//! Branch.
	Branch * m_branch;
	//! Parent.
	Leaf * q;
	//! Entity counter.
	quint64 & m_entityCounter;
	//! Use instance rendering?
	bool m_useInstanceRendering;
	//! Start rotation around branch vector.
	float m_startBranchRot;
	//! Distortion of rotation around leaf normal.
	float m_leafDistRot;
	//! Fall and die mode?
	bool m_fallAndDie;
}; // class LeafPrivate

void
LeafPrivate::init()
{
	q->addComponent( m_mesh );

	auto material = std::make_unique< QPhongMaterial > ();

	material->setDiffuse( Qt::darkGreen );

	m_material = material.get();

	q->addComponent( material.release() );

	auto transform = std::make_unique< Qt3DCore::QTransform > ();

	transform->setTranslation( *m_endBranchPos );

	m_transform = transform.get();

	q->addComponent( transform.release() );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, 360.0f );

	m_fallAngle = dis( gen );
}


//
// Leaf
//

Leaf::Leaf( const QVector3D & startBranchPos,
	const QVector3D & endBranchPos, Qt3DRender::QMesh * mesh,
	QTimer * animationTimer, Branch * parentBranch, quint64 & entityCounter,
	Qt3DCore::QNode * parent, bool useInstanceRendering )
	:	Qt3DCore::QEntity( parent )
	,	d( new LeafPrivate( startBranchPos, endBranchPos, mesh,
			animationTimer, parentBranch, this, entityCounter,
			useInstanceRendering ) )
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

	rotate( d->m_startBranchRot );
}

void
Leaf::rotate( float angle )
{
	d->m_startBranchRot = angle;

	QVector3D branch = ( *( d->m_endBranchPos ) -
		*( d->m_startBranchPos ) ).normalized();

	if( branch.isNull() )
		branch = QVector3D( 0.0f, 1.0f, 0.0f );

	const QVector3D leaf( 0.0f, 1.0f, 0.0f );

	const QVector3D axis = QVector3D::crossProduct( leaf, branch ).normalized();

	const float cosPlainAngle = QVector3D::dotProduct( leaf, branch );

	if( d->m_leafDistRot < 0.0f || d->m_fallAndDie )
	{
		std::random_device rd;
		std::mt19937 gen( rd() );
		std::uniform_real_distribution< float > dis( 0.0f, c_leafAngle );

		d->m_leafDistRot = dis( gen );
	}

	float plainAngle = qRadiansToDegrees( std::acos( cosPlainAngle ) );

	const QQuaternion quat = Qt3DCore::QTransform::fromAxesAndAngles(
		axis, plainAngle,
		QVector3D::crossProduct( branch, QVector3D( 0.0f, 0.0f, 1.0f ) ).normalized(),
		d->m_leafDistRot,
		branch, angle );

	d->m_transform->setRotation( quat );
}

void
Leaf::fallAndDie()
{
	d->m_fallAndDie = true;

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
	{
		deleteLater();

		if( d->m_useInstanceRendering )
			d->m_mesh->setInstanceCount( d->m_mesh->instanceCount() - 1 );
	}
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
