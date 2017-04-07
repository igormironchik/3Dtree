
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
#include "branch.hpp"
#include "constants.hpp"
#include "leaf.hpp"

// Qt include.
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>

// C++ include.
#include <random>
#include <cmath>


//
// BranchPrivate
//

class BranchPrivate {
public:
	BranchPrivate( const QVector3D & startParentPos,
		const QVector3D & endParentPos, Branch * parent )
		:	m_mesh( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_material( Q_NULLPTR )
		,	m_startParentPos( startParentPos )
		,	m_endParentPos( endParentPos )
		,	m_startPos( 0.0f, 0.0f, 0.0f )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Mesh.
	Qt3DExtras::QConeMesh * m_mesh;
	//! Transform.
	Qt3DCore::QTransform * m_transform;
	//! Material.
	Qt3DExtras::QPhongMaterial * m_material;
	//! Start parent pos.
	const QVector3D & m_startParentPos;
	//! End parent pos.
	const QVector3D & m_endParentPos;
	//! Start pos.
	QVector3D m_startPos;
	//! End pos.
	QVector3D m_endPos;
	//! Parent.
	Branch * q;
}; // class BranchPrivate

void
BranchPrivate::init()
{
	m_mesh = new Qt3DExtras::QConeMesh( q );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f,
		c_branchLengthDistortion );

	m_mesh->setBottomRadius( c_branchBottomRadius );
	m_mesh->setTopRadius( c_branchTopRadius );
	m_mesh->setHasBottomEndcap( true );
	m_mesh->setHasTopEndcap( true );
	m_mesh->setLength( c_branchLength + dis( gen ) );
	m_mesh->setRings( 20 );
	m_mesh->setSlices( 10 );

	q->addComponent( m_mesh );

	m_transform = new Qt3DCore::QTransform( q );

	m_transform->setTranslation( m_endParentPos );

	m_startPos = m_endParentPos;

	m_endPos = m_startPos + QVector3D( 0.0f, c_branchLength, 0.0f );

	q->addComponent( m_transform );

	m_material = new Qt3DExtras::QPhongMaterial( q );

	m_material->setDiffuse( Qt::darkGray );

	q->addComponent( m_material );
}


//
// Branch
//

Branch::Branch( const QVector3D & startParentPos,
	const QVector3D & endParentPos, Qt3DCore::QNode * parent )
	:	Qt3DCore::QEntity( parent )
	,	d( new BranchPrivate( startParentPos, endParentPos, this ) )
{
	d->init();
}

Branch::~Branch()
{
}

void
Branch::placeOnTopAndParallel()
{
	const QVector3D parent = ( d->m_endParentPos - d->m_startParentPos )
		.normalized();
	const QVector3D b( 0.0f, 1.0f, 0.0f );

	const QVector3D axis = QVector3D::crossProduct( parent, b );

	const float cosAngle = QVector3D::dotProduct( parent, b );

	const float angle = std::acos( cosAngle ) * c_degInRad;

	d->m_transform->setRotation( Qt3DCore::QTransform::fromAxisAndAngle( axis,
		angle ) );

	updatePosition();
}

void
Branch::rotate( float angle )
{
	const QVector3D parent = ( d->m_endParentPos - d->m_startParentPos )
		.normalized();
	const QVector3D b( 0.0f, 1.0f, 0.0f );

	const QVector3D axis = QVector3D::crossProduct( parent, b );

	const float cosAngle = QVector3D::dotProduct( parent, b );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, c_maxBranchAngle );

	const float plainAngle = std::acos( cosAngle ) * c_degInRad
		+ dis( gen ) - 90.0f;

	QQuaternion q = Qt3DCore::QTransform::fromAxisAndAngle( axis,
		plainAngle );

	q += Qt3DCore::QTransform::fromAxisAndAngle( parent, angle );

	d->m_transform->setRotation( q );
}

void
Branch::setAge( float age )
{

}

void
Branch::updatePosition()
{
	d->m_transform->setTranslation( d->m_endParentPos );
}
