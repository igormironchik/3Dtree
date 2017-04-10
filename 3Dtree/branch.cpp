
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

#include <QList>
#include <QtMath>

// C++ include.
#include <random>
#include <cmath>

#include <Qt3DExtras/QSphereMesh>


//
// BranchPrivate
//

class BranchPrivate {
public:
	BranchPrivate( const QVector3D & startParentPos,
		const QVector3D & endParentPos,
		float parentRadius, bool continuation, Branch * parent )
		:	m_mesh( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_material( Q_NULLPTR )
		,	m_startParentPos( startParentPos )
		,	m_endParentPos( endParentPos )
		,	m_parentRadius( parentRadius )
		,	m_continuation( continuation )
		,	m_startPos( endParentPos )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! Place on top of parent and make parallel to the parent.
	void placeOnTopAndParallel();

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
	//! Parent radius.
	float m_parentRadius;
	//! Is this branch a continuation of the parent?
	bool m_continuation;
	//! Start pos.
	const QVector3D & m_startPos;
	//! End pos.
	QVector3D m_endPos;
	//! Leafs.
	QList< QPair< Leaf*, bool > > m_leafs;
	//! Child branches.
	QList< Branch* > m_children;
	//! Parent.
	Branch * q;
}; // class BranchPrivate

void
BranchPrivate::init()
{
	m_mesh = new Qt3DExtras::QConeMesh( q );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > ldis( 0.0f,
		c_branchLengthDistortion );

	std::uniform_real_distribution< float > rdis( 0.0f, c_branchDistortion );

	if( m_continuation )
		m_mesh->setBottomRadius( m_parentRadius );
	else
		m_mesh->setBottomRadius( m_parentRadius - rdis( gen ) );

	m_mesh->setTopRadius( m_mesh->bottomRadius() - c_branchRadiusDelta );

	m_mesh->setHasBottomEndcap( true );
	m_mesh->setHasTopEndcap( true );

	m_mesh->setLength( c_branchLength + ldis( gen ) );

	m_mesh->setRings( 20 );
	m_mesh->setSlices( 10 );

	q->addComponent( m_mesh );

	m_transform = new Qt3DCore::QTransform( q );

	m_transform->setTranslation( m_endParentPos );

	m_endPos = m_startPos + QVector3D( 0.0f, m_mesh->length(), 0.0f );

	q->addComponent( m_transform );

	m_material = new Qt3DExtras::QPhongMaterial( q );

	m_material->setDiffuse( Qt::darkGray );

	q->addComponent( m_material );

	if( m_continuation )
		placeOnTopAndParallel();

	// Create leafs.
	for( quint8 i = 0; i < c_leafsCount; ++i )
	{
		m_leafs.push_back( qMakePair( new Leaf( m_startPos, m_endPos,
			q->parentEntity() ), false ) );
		m_leafs.last().first->updatePosition();
		m_leafs.last().first->setAge( 0.0f );
	}
}

void
BranchPrivate::placeOnTopAndParallel()
{
	const QVector3D parent = ( m_endParentPos - m_startParentPos )
		.normalized();
	const QVector3D b = QVector3D( 0.0f, 1.0f, 0.0f ).normalized();

	const QVector3D axis = QVector3D::crossProduct( parent, b );

	const float cosAngle = QVector3D::dotProduct( parent, b );

	const float angle = - qRadiansToDegrees( std::acos( cosAngle ) );

	m_transform->setRotation( Qt3DCore::QTransform::fromAxisAndAngle( axis,
		angle ) );

	q->updatePosition();
}


//
// Branch
//

Branch::Branch( const QVector3D & startParentPos,
	const QVector3D & endParentPos,
	float parentRadius, bool continuation, Qt3DCore::QEntity * parent )
	:	Qt3DCore::QEntity( parent )
	,	d( new BranchPrivate( startParentPos, endParentPos,
			parentRadius, continuation, this ) )
{
	d->init();
}

Branch::~Branch()
{
}

void
Branch::rotate( float angle )
{
	const QVector3D parent = ( d->m_endParentPos - d->m_startParentPos )
		.normalized();
	const QVector3D b( 0.0f, 1.0f, 0.0f );

	QVector3D axis = QVector3D::crossProduct( parent, b );

	if( axis.isNull() )
		axis = QVector3D( 1.0f, 0.0f, 0.0f );

	const float cosAngle = QVector3D::dotProduct( parent, b );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, c_maxBranchAngle );

	const float plainAngle = 90.0f - qRadiansToDegrees( std::acos( cosAngle ) )
		- dis( gen );

	const QQuaternion q = Qt3DCore::QTransform::fromAxesAndAngles( axis,
		plainAngle, parent, angle );

	d->m_transform->setRotation( q );
}

void
Branch::setAge( float age )
{
	float tmp = age;
	float i = 0.0f;

	for( ; tmp >= 1.0f; tmp -= 1.0f )
		i += 1.0f;

	if( tmp <= 0.5f )
		tmp *= 2.0f;
	else
		tmp = 1.0f;

	const float summerAge = i + tmp;

	d->m_transform->setScale( summerAge <= 1.0 ? summerAge :
		 1.0f + summerAge / ( 100.0f / 3.0f ) );

	updatePosition();

	if( age < 1.0f )
	{
		for( auto it = d->m_leafs.begin(), last = d->m_leafs.end();
			it != last; ++it )
		{
			// Spring.
			if( age <= 0.25f )
			{
				(*it).first->setAge( age * 4.0f );
				(*it).first->updatePosition();
			}
			else if( age > 0.25f && age <= 0.5f )
				(*it).first->updatePosition();
			// Autumn.
			else if( age > 0.5f && age <= 0.75f )
			{
				if( !(*it).second )
				{
					std::random_device rd;
					std::mt19937 gen( rd() );
					std::uniform_real_distribution< float > autumn( age,
						0.75f );

					if( autumn( gen ) > 0.63f )
					{
						static const float segmentLength = 1.0f;

						std::uniform_real_distribution< float > dis( 0.0f,
							segmentLength );

						const float pdist = dis( gen );
						const float ratio = pdist / segmentLength;

						static const QColor c1( Qt::yellow );
						static const QColor c2( Qt::red );

						const int red = (int)( ratio * c1.redF() +
							( 1.0f - ratio ) * c2.redF() );
						const int green = (int)( ratio * c1.greenF() +
							( 1.0f - ratio ) * c2.greenF() );
						const int blue = (int)( ratio * c1.blueF() +
							( 1.0f - ratio ) * c2.blueF() );

						(*it).first->setColor( QColor( red, green, blue ) );

						(*it).second = true;
					}
				}
			}
			// Winter.
			else if( age > 0.75f )
				(*it).first->deleteLater();
		}

		if( age > 0.75f )
			d->m_leafs.clear();
	}

	if( !d->m_children.isEmpty() )
	{
		for( const auto & b : qAsConst( d->m_children ) )
			b->setAge( age - 1.0f );
	}
	else if( age >= 1.0f )
	{
		if( c_hasContinuationBranch )
		{
			d->m_children.push_back( new Branch( startPos(),
				endPos(), topRadius(), true, parentEntity() ) );
			d->m_children.last()->updatePosition();
			d->m_children.last()->setAge( 0.0f );
		}

		const quint8 count = c_childBranchesCount -
			( c_hasContinuationBranch ? 1 : 0 );

		std::random_device rd;
		std::mt19937 gen( rd() );
		std::uniform_real_distribution< float > dis( 0.0f,
			c_branchRotationDistortion );

		float angle = dis( gen );

		for( quint8 i = 0; i < count; ++i )
		{
			d->m_children.push_back( new Branch( startPos(),
				endPos(), topRadius(), false, parentEntity() ) );
			d->m_children.last()->rotate( angle );
			d->m_children.last()->updatePosition();
			d->m_children.last()->placeLeafs();
			d->m_children.last()->setAge( 0.0f );

			angle += 360.0f / (float) count;
		}
	}
}

void
Branch::updatePosition()
{
	QVector3D v( 0.0f, 0.0f, 0.0f );

	const QVector3D start( 0.0f, 0.0f, 0.0f );
	const QVector3D end( 0.0f, length(), 0.0f );

	v = ( d->m_transform->matrix() *
		( end - start ) ).normalized();
	const float m = ( !qFuzzyIsNull( v.length() ) ?
		length() / 2.0f / v.length() : 1.0f );
	v *= m;

	d->m_transform->setTranslation( d->m_endParentPos + v );

	const QVector3D tmp( 0.0f, d->m_mesh->length() / 2.0f, 0.0f );

	d->m_endPos = d->m_transform->matrix() * tmp;
}

void
Branch::placeLeafs()
{
	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > rotdis( 0.0f,
		c_leafRotationDistortion );

	float startLeafAngle = rotdis( gen );

	for( quint8 i = 0; i < c_leafsCount; ++i )
	{
		d->m_leafs.at( i ).first->rotate( startLeafAngle );
		d->m_leafs.at( i ).first->updatePosition();
		d->m_leafs.at( i ).first->setAge( 0.0f );

		startLeafAngle += 360.0f / (float) c_leafsCount;
	}
}

const QVector3D &
Branch::startPos() const
{
	return d->m_startPos;
}

const QVector3D &
Branch::endPos() const
{
	return d->m_endPos;
}

float
Branch::topRadius() const
{
	return d->m_mesh->topRadius() * d->m_transform->scale();
}

float
Branch::length() const
{
	return d->m_mesh->length() * d->m_transform->scale();
}
