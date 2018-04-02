
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
#include <Qt3DRender/QMesh>

#include <QList>
#include <QtMath>

// C++ include.
#include <random>
#include <cmath>


//
// LeafData
//

struct LeafData Q_DECL_FINAL {
	explicit LeafData( Leaf * l )
		:	m_leaf( l )
		,	m_autumn( false )
		,	m_deleted( false )
	{
	}

	bool operator == ( const LeafData & leaf ) const
	{
		return ( m_leaf == leaf.m_leaf );
	}

	Leaf * m_leaf;
	bool m_autumn;
	bool m_deleted;
}; // struct LeafData


//
// BranchPrivate
//

class BranchPrivate {
public:
	BranchPrivate( const QVector3D & startParentPos,
		const QVector3D & endParentPos, quint16 & age,
		float parentRadius, bool continuation, bool isTree,
		Qt3DExtras::QPhongMaterial * material,
		Qt3DRender::QMesh * leafMesh,
		QTimer * animationTimer,
		Branch * parent, Branch * parentBranch,
		bool firstBranch, quint64 & entityCounter,
		bool useInstanceRendering, bool enableDeath )
		:	m_mesh( Q_NULLPTR )
		,	m_transform( Q_NULLPTR )
		,	m_material( material )
		,	m_length( 0.0f )
		,	m_startParentPos( startParentPos )
		,	m_endParentPos( endParentPos )
		,	m_treeAge( age )
		,	m_age( 0 )
		,	m_parentRadius( parentRadius )
		,	m_continuation( continuation )
		,	m_isTree( isTree )
		,	m_startPos( endParentPos )
		,	m_leafMesh( leafMesh )
		,	m_animationTimer( animationTimer )
		,	m_firstBranch( firstBranch )
		,	m_useInstanceRendering( useInstanceRendering )
		,	m_enableDeath( enableDeath )
		,	m_parentBranch( parentBranch )
		,	q( parent )
		,	m_entityCounter( entityCounter )
	{
		++m_entityCounter;
	}

	~BranchPrivate()
	{
		for( const auto * b : qAsConst( m_children ) )
			delete b;

		m_children.clear();

		for( const auto & l : qAsConst( m_leafs ) )
			delete l.m_leaf;

		m_leafs.clear();

		--m_entityCounter;
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
	//! Start length.
	float m_length;
	//! Start parent pos.
	const QVector3D & m_startParentPos;
	//! End parent pos.
	const QVector3D & m_endParentPos;
	//! Age of the tree.
	quint16 & m_treeAge;
	//! Age of the branch.
	quint16 m_age;
	//! Parent radius.
	float m_parentRadius;
	//! Is this branch a continuation of the parent?
	bool m_continuation;
	//! Is it a tree?
	bool m_isTree;
	//! Start pos.
	const QVector3D & m_startPos;
	//! End pos.
	QVector3D m_endPos;
	//! Leafs.
	QList< LeafData > m_leafs;
	//! Child branches.
	QList< Branch* > m_children;
	//! Leaf mesh.
	Qt3DRender::QMesh * m_leafMesh;
	//! Animation timer.
	QTimer * m_animationTimer;
	//! Is this a first branch.
	bool m_firstBranch;
	//! Use instance rendering?
	bool m_useInstanceRendering;
	//! Enable death?
	bool m_enableDeath;
	//! Real parent.
	Branch * m_parentBranch;
	//! Parent.
	Branch * q;
	//! Entity counter.
	quint64 & m_entityCounter;
}; // class BranchPrivate

void
BranchPrivate::init()
{
	QScopedPointer< Qt3DExtras::QConeMesh > coneMesh( new Qt3DExtras::QConeMesh );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > ldis( 0.0f,
		c_branchLengthDistortion );

	std::uniform_real_distribution< float > rdis( 0.0f, c_branchDistortion );

	if( m_continuation )
		coneMesh->setBottomRadius( m_parentRadius );
	else
		coneMesh->setBottomRadius( m_parentRadius - rdis( gen ) );

	coneMesh->setTopRadius( coneMesh->bottomRadius() - c_branchRadiusDelta );

	coneMesh->setHasBottomEndcap( true );
	coneMesh->setHasTopEndcap( true );

	m_length = c_branchLength + ( m_firstBranch ? 0.0f : ldis( gen ) );

	coneMesh->setLength( m_length );

	coneMesh->setRings( 20 );
	coneMesh->setSlices( 10 );

	m_mesh = coneMesh.data();

	q->addComponent( coneMesh.take() );

	QScopedPointer< Qt3DCore::QTransform > transform( new Qt3DCore::QTransform );

	transform->setTranslation( m_endParentPos );

	m_endPos = m_startPos + QVector3D( 0.0f, m_mesh->length(), 0.0f );

	m_transform = transform.data();

	q->addComponent( transform.take() );

	q->addComponent( m_material );

	// If this branch is continuation branch then place it on top and parallel.
	if( m_continuation )
		placeOnTopAndParallel();

	if( m_useInstanceRendering )
		m_leafMesh->setInstanceCount( m_leafMesh->instanceCount() +
			c_leafsCount );

	// Create leafs.
	for( quint8 i = 0; i < c_leafsCount; ++i )
	{
		m_leafs.push_back( LeafData( new Leaf( m_startPos, m_endPos,
			m_leafMesh, m_animationTimer, q, m_entityCounter,
			q->parentEntity(), m_useInstanceRendering ) ) );
		m_leafs.last().m_leaf->updatePosition();
		m_leafs.last().m_leaf->setAge( 0.0f );

		QObject::connect( m_leafs.last().m_leaf, &Leaf::nodeDestroyed,
			q, &Branch::leafDeleted );
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
	const QVector3D & endParentPos, quint16 & age,
	float parentRadius, bool continuation, bool isTree,
	Qt3DExtras::QPhongMaterial * material,
	Qt3DRender::QMesh * leafMesh,
	QTimer * animationTimer,
	Branch * parentBranch,
	quint64 & entityCounter,
	Qt3DCore::QEntity * parent,
	bool firstBranch,
	bool useInstanceRendering,
	bool enableDeath )
	:	Qt3DCore::QEntity( parent )
	,	d( new BranchPrivate( startParentPos, endParentPos, age,
			parentRadius, continuation, isTree, material,
			leafMesh, animationTimer, this, parentBranch,
			firstBranch, entityCounter, useInstanceRendering, enableDeath ) )
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

	QVector3D axis = QVector3D::crossProduct( b, parent ).normalized();

	if( axis.isNull() )
		axis = QVector3D( 1.0f, 0.0f, 0.0f );

	const float cosAngle = QVector3D::dotProduct( b, parent );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( 0.0f, c_maxBranchAngle );

	const float plainAngle = qRadiansToDegrees( std::acos( cosAngle ) ) + 90.0f
		- dis( gen );

	const QQuaternion q = Qt3DCore::QTransform::fromAxesAndAngles( axis,
		plainAngle, parent, angle );

	d->m_transform->setRotation( q );
}

void
Branch::setAge( float age )
{
	d->m_age = static_cast< quint16 > ( qRound( age ) );

	float tmp = age;
	float i = 0.0f;

	for( ; tmp >= 1.0f; tmp -= 1.0f )
		i += 1.0f;

	if( tmp <= 0.25f )
		tmp *= 4.0f;
	else
		tmp = 1.0f;

	const float summerAge = i + tmp;

	static const float c_deepAutumn = 0.96f;

	d->m_transform->setScale( summerAge <= 1.0 ? summerAge :
		 1.0f + summerAge / ( 100.0f / c_branchScale ) );

	d->m_mesh->setLength( d->m_length +
		d->m_length * summerAge / ( 100.0f / c_branchLengthMultiplicator ) /
		// Tree trunk grows faster, branches grow slower.
		( !d->m_isTree ? c_branchSlower : 1.0f ) *
		// First tree trunk branch grows even faster.
		( d->m_firstBranch ? c_firstBranchGrowsFaster : 1.0f ) );

	updatePosition();

	if( age < 1.0f )
	{
		for( auto it = d->m_leafs.begin(), last = d->m_leafs.end();
			it != last; ++it )
		{
			// Spring.
			if( age <= 0.25f )
			{
				(*it).m_leaf->setAge( age * 4.0f );
				(*it).m_leaf->updatePosition();
			}
			else if( age > 0.25f && age <= 0.5f )
				(*it).m_leaf->updatePosition();
			// Autumn.
			else if( age > 0.5f && age <= 0.75f )
			{
				if( !(*it).m_autumn )
				{
					std::random_device rd;
					std::mt19937 gen( rd() );
					std::uniform_real_distribution< float > autumn( age,
						0.75f );

					if( autumn( gen ) > 0.63f )
					{
						(*it).m_leaf->setColor( Leaf::autumnColor() );

						(*it).m_autumn = true;
					}
				}
			}
			// Deep autumn.
			else if( age > 0.75f )
			{
				if( !(*it).m_deleted )
				{
					std::random_device rd;
					std::mt19937 gen( rd() );
					std::uniform_real_distribution< float > autumn( age,
						0.97f );

					if( autumn( gen ) > c_deepAutumn )
					{
						(*it).m_leaf->fallAndDie();

						(*it).m_deleted = true;
					}
				}
			}
		}

		if( age > c_deepAutumn )
		{
			for( auto it = d->m_leafs.begin(), last = d->m_leafs.end();
				it != last; ++it )
			{
				if( !(*it).m_deleted )
					(*it).m_leaf->fallAndDie();
			}

			d->m_leafs.clear();
		}
	}

	if( !d->m_children.isEmpty() )
	{
		auto tmp = d->m_children;

		for( const auto & b : qAsConst( tmp ) )
			b->setAge( age - 1.0f );
	}
	else if( age >= 1.0f )
	{
		if( c_hasContinuationBranch )
		{
			d->m_children.push_back( new Branch( startPos(),
				endPos(), d->m_treeAge,
				topRadius(), true, d->m_isTree, d->m_material,
				d->m_leafMesh, d->m_animationTimer, this, d->m_entityCounter,
				parentEntity(), false, d->m_useInstanceRendering,
				d->m_enableDeath ) );

			d->m_children.last()->updatePosition();
			d->m_children.last()->placeLeafs();
			d->m_children.last()->setAge( 0.0f );

			connect( d->m_children.last(), &Branch::nodeDestroyed,
				this, &Branch::childBranchDeleted );
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
				endPos(), d->m_treeAge, topRadius(), false, false,
				d->m_material, d->m_leafMesh,
				d->m_animationTimer, this, d->m_entityCounter,
				parentEntity(), false, d->m_useInstanceRendering,
				d->m_enableDeath ) );
			d->m_children.last()->rotate( angle );
			d->m_children.last()->updatePosition();
			d->m_children.last()->placeLeafs();
			d->m_children.last()->setAge( 0.0f );

			connect( d->m_children.last(), &Branch::nodeDestroyed,
				this, &Branch::childBranchDeleted );

			angle += 360.0f / (float) count;
		}
	}

	// Death.
	if( d->m_enableDeath && d->m_age > 1 )
	{
		if( d->m_age < c_minDeathThreeshold || d->m_age > c_maxDeathThreeshold )
		{
			std::random_device rd;
			std::mt19937 gen( rd() );
			std::normal_distribution< float > dis( 0.0f, 0.5f );

			if( !d->m_isTree )
			{
				auto v = dis( gen );

				if( v >= c_deathProbability )
				{
					for( const auto * b : qAsConst( d->m_children ) )
						delete b;

					d->m_children.clear();

					deleteLater();
				}
			}
		}
	}
}

void
Branch::updatePosition()
{
	d->m_transform->setTranslation( d->m_endParentPos );

	QVector3D end = QVector3D( 0.0f, d->m_mesh->length() / 2.0f, 0.0f );
	end = d->m_transform->matrix() * end;

	d->m_transform->setTranslation( end );

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
		d->m_leafs.at( i ).m_leaf->rotate( startLeafAngle );
		d->m_leafs.at( i ).m_leaf->updatePosition();
		d->m_leafs.at( i ).m_leaf->setAge( 0.0f );

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

void
Branch::childBranchDeleted()
{
	d->m_children.removeOne( static_cast< Branch* > ( sender() ) );
}

void
Branch::leafDeleted()
{
	d->m_leafs.removeOne( LeafData( static_cast< Leaf* > ( sender() ) ) );
}
