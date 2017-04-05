
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
#include "leaf_mesh.hpp"
#include "constants.hpp"

// Qt include.
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>

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
		const QVector3D & endBranchPos, Leaf * parent )
		:	m_mesh( Q_NULLPTR )
		,	m_material( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_startBranchPos( startBranchPos )
		,	m_endBranchPos( endBranchPos )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Mesh.
	LeafMesh * m_mesh;
	//! Material.
	QPhongMaterial * m_material;
	//! Transform.
	Qt3DCore::QTransform * m_transform;
	//! Start branch position.
	const QVector3D & m_startBranchPos;
	//! End branch position.
	const QVector3D & m_endBranchPos;
	//! Parent.
	Leaf * q;
}; // class LeafPrivate

void
LeafPrivate::init()
{
	m_mesh = new LeafMesh( q );

	q->addComponent( m_mesh );

	m_material = new QPhongMaterial( q );

	m_material->setDiffuse( Qt::darkGreen );

	q->addComponent( m_material );

	m_transform = new Qt3DCore::QTransform( q );

	m_transform->setTranslation( m_endBranchPos );

	q->addComponent( m_transform );

	q->rotate( 0.0f );
}


//
// Leaf
//

Leaf::Leaf( const QVector3D & startBranchPos,
	const QVector3D & endBranchPos, Qt3DCore::QNode * parent )
	:	Qt3DCore::QEntity( parent )
	,	d( new LeafPrivate( startBranchPos, endBranchPos, this ) )
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
	d->m_transform->setTranslation( d->m_endBranchPos );
}

void
Leaf::rotate( float angle )
{
	const QVector3D branch = ( d->m_endBranchPos -
		d->m_startBranchPos ).normalized();
	const QVector3D leaf( 0.0f, 1.0f, 0.0f );

	const QVector3D axis = QVector3D::crossProduct( branch, leaf );

	const float cosPlainAngle = QVector3D::dotProduct( branch, leaf );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, c_leafAngle );

	const float plainAngle = std::acos( cosPlainAngle ) * c_degInRad +
		dis( gen );

	QQuaternion quat = Qt3DCore::QTransform::fromAxisAndAngle( axis,
		plainAngle );

	quat += Qt3DCore::QTransform::fromAxisAndAngle( branch, angle );

	d->m_transform->setRotation( quat );
}
