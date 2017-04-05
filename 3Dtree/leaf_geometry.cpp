
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
#include "leaf_geometry.hpp"

// Qt include.
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QBufferDataGenerator>

// C++ include.
#include <random>


using namespace Qt3DRender;

namespace /* anonymous */ {

//! Number of vertexes.
static const int c_vertsNumber = 9;
//! Size of element - vec3 pos, vec2 texCoord, vec3 normal.
static const quint32 c_elementSize = 3 + 2 + 3;
//! Stride.
static const quint32 c_stride = c_elementSize * sizeof( float );
//! Faces number.
static const int c_faces = 8;
//! Indicies number.
static const int c_indices = c_faces * 3;


//
// initNormal
//

//! Init normal.
void initNormal( float * fptr )
{
	*fptr++ = 0.0f;
	*fptr++ = 1.0f;
	*fptr++ = 0.0f;
} // initNormal

/*
	  v
	  |     |
	  |   0 *------------z
	  |1 *  |2 *
	  |     |
	3 *   4 *   5 *
	  |     |
	  |6 *  |7 *
	   ---8-*------------u
	        |
	        x
*/


//
// createLeafVertexData
//

//! Create leaf vertex data.
QByteArray createLeafVertexData()
{
	QByteArray bufferBytes;
	bufferBytes.resize( c_stride * c_vertsNumber );

	float * fptr = reinterpret_cast< float* > ( bufferBytes.data() );

	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > dis( -0.05f, 0.05f );

	// 0
	// position
	*fptr++ = 0.0f;
	*fptr++ = 0.0f;
	*fptr++ = 0.0f;

	// texture coordinates
	*fptr++ = 0.5f;
	*fptr++ = 1.0f;

	initNormal( fptr );

	// 1
	*fptr++ = 0.17f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = -0.17f + dis( gen );

	*fptr++ = 0.25f;
	*fptr++ = 0.83f;

	initNormal( fptr );

	// 2
	*fptr++ = 0.17f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = 0.17f + dis( gen );

	*fptr++ = 0.75f;
	*fptr++ = 0.83f;

	initNormal( fptr );

	// 3
	*fptr++ = 0.5f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = -0.33f + dis( gen );

	*fptr++ = 0.0f;
	*fptr++ = 0.5f;

	initNormal( fptr );

	// 4
	*fptr++ = 0.5f;
	*fptr++ = 0.0f;
	*fptr++ = 0.0f;

	*fptr++ = 0.5f;
	*fptr++ = 0.5f;

	initNormal( fptr );

	// 5
	*fptr++ = 0.5f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = 0.33f + dis( gen );

	*fptr++ = 1.0f;
	*fptr++ = 0.5f;

	initNormal( fptr );

	// 6
	*fptr++ = 0.83f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = -0.17f + dis( gen );

	*fptr++ = 0.25f;
	*fptr++ = 0.17f;

	initNormal( fptr );

	// 7
	*fptr++ = 0.83f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = 0.17f + dis( gen );

	*fptr++ = 0.75f;
	*fptr++ = 0.17f;

	initNormal( fptr );

	// 8
	*fptr++ = 1.0f + dis( gen );
	*fptr++ = 0.0f;
	*fptr++ = 0.0f + dis( gen );

	*fptr++ = 0.5f;
	*fptr++ = 0.0f;

	initNormal( fptr );

	return bufferBytes;
} // createLeafVertexData


//
// createLeafIndexData
//

//! Create leaf index data.
QByteArray createLeafIndexData()
{
	QByteArray indexBytes;
	indexBytes.resize( c_indices * sizeof( quint16 ) );
	quint16 * indexPtr = reinterpret_cast< quint16* > ( indexBytes.data() );

	// 1
	*indexPtr++ = 0;
	*indexPtr++ = 1;
	*indexPtr++ = 4;

	// 2
	*indexPtr++ = 0;
	*indexPtr++ = 2;
	*indexPtr++ = 4;

	// 3
	*indexPtr++ = 2;
	*indexPtr++ = 5;
	*indexPtr++ = 4;

	// 4
	*indexPtr++ = 5;
	*indexPtr++ = 7;
	*indexPtr++ = 4;

	// 5
	*indexPtr++ = 7;
	*indexPtr++ = 8;
	*indexPtr++ = 4;

	// 6
	*indexPtr++ = 8;
	*indexPtr++ = 6;
	*indexPtr++ = 4;

	// 7
	*indexPtr++ = 6;
	*indexPtr++ = 3;
	*indexPtr++ = 4;

	// 8
	*indexPtr++ = 3;
	*indexPtr++ = 1;
	*indexPtr++ = 4;

	return indexBytes;
} // createLeafIndexData


//
// LeafVertexBufferFunctor
//

//! Leaf vertex buffer functor.
class LeafVertexBufferFunctor Q_DECL_FINAL
	:	public QBufferDataGenerator
{
public:
	LeafVertexBufferFunctor()
	{
	}

	~LeafVertexBufferFunctor()
	{
	}

	QByteArray operator () () Q_DECL_OVERRIDE
	{
		return createLeafVertexData();
	}

	bool operator == ( const QBufferDataGenerator & other ) const Q_DECL_OVERRIDE
	{
		const LeafVertexBufferFunctor * otherFunctor =
			functor_cast< LeafVertexBufferFunctor > ( &other );

        if( otherFunctor != Q_NULLPTR )
			return true;
		else
			return false;
	}

	QT3D_FUNCTOR( LeafVertexBufferFunctor )
}; // LeafVertexBufferFunctor


//
// LeafIndexBufferFunctor
//

//! Leaf index buffer functor.
class LeafIndexBufferFunctor Q_DECL_FINAL
	:	public QBufferDataGenerator
{
public:
	LeafIndexBufferFunctor()
	{
	}

	~LeafIndexBufferFunctor()
	{
	}

	QByteArray operator () () Q_DECL_OVERRIDE
	{
		return createLeafIndexData();
	}

	bool operator == ( const QBufferDataGenerator & other ) const Q_DECL_OVERRIDE
	{
		const LeafIndexBufferFunctor * otherFunctor =
			functor_cast< LeafIndexBufferFunctor > ( &other );

        if( otherFunctor != Q_NULLPTR )
			return true;
		else
			return false;
	}

	QT3D_FUNCTOR( LeafIndexBufferFunctor )
}; // LeafIndexBufferFunctor

} // namespace anonymous


//
// LeafGeometryPrivate
//

class LeafGeometryPrivate {
public:
	LeafGeometryPrivate( LeafGeometry * parent )
		:	m_positionsAttr( Q_NULLPTR )
		,	m_texCoordAttr( Q_NULLPTR )
		,	m_normalsAttr( Q_NULLPTR )
		,	m_indexAttribute( Q_NULLPTR )
		,	m_vertexBuffer( Q_NULLPTR )
		,	m_indexBuffer( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Positions attribute.
	QAttribute * m_positionsAttr;
	//! Texture coordinates attribute.
	QAttribute * m_texCoordAttr;
	//! Normals attribute.
	QAttribute * m_normalsAttr;
	//! Index attribute.
	QAttribute * m_indexAttribute;
	//! Vertex buffer.
	Qt3DRender::QBuffer * m_vertexBuffer;
	//! Indexies buffer.
	Qt3DRender::QBuffer * m_indexBuffer;
	//! Parent.
	LeafGeometry * q;
}; // class LeafGeometryPrivate

void
LeafGeometryPrivate::init()
{
	m_positionsAttr = new QAttribute( q );
	m_normalsAttr = new QAttribute( q );
	m_texCoordAttr = new QAttribute( q );
	m_indexAttribute = new QAttribute( q );
	m_vertexBuffer = new Qt3DRender::QBuffer( Qt3DRender::QBuffer::VertexBuffer,
		q );
	m_indexBuffer = new Qt3DRender::QBuffer( Qt3DRender::QBuffer::IndexBuffer,
		q );

	m_positionsAttr->setName( QAttribute::defaultPositionAttributeName() );
	m_positionsAttr->setVertexBaseType( QAttribute::Float );
	m_positionsAttr->setVertexSize( 3 );
	m_positionsAttr->setAttributeType( QAttribute::VertexAttribute );
	m_positionsAttr->setBuffer( m_vertexBuffer );
	m_positionsAttr->setByteStride( c_stride );
	m_positionsAttr->setCount( c_vertsNumber );

	m_texCoordAttr->setName( QAttribute::defaultTextureCoordinateAttributeName() );
	m_texCoordAttr->setVertexBaseType( QAttribute::Float );
	m_texCoordAttr->setVertexSize( 2 );
	m_texCoordAttr->setAttributeType( QAttribute::VertexAttribute );
	m_texCoordAttr->setBuffer( m_vertexBuffer );
	m_texCoordAttr->setByteStride( c_stride );
	m_texCoordAttr->setByteOffset( 3 * sizeof( float ) );
	m_texCoordAttr->setCount( c_vertsNumber );

	m_normalsAttr->setName( QAttribute::defaultNormalAttributeName() );
	m_normalsAttr->setVertexBaseType( QAttribute::Float );
	m_normalsAttr->setVertexSize( 3 );
	m_normalsAttr->setAttributeType( QAttribute::VertexAttribute );
	m_normalsAttr->setBuffer( m_vertexBuffer );
	m_normalsAttr->setByteStride( c_stride );
	m_normalsAttr->setByteOffset( 5 * sizeof( float ) );
	m_normalsAttr->setCount( c_vertsNumber );

	m_indexAttribute->setAttributeType( QAttribute::IndexAttribute );
	m_indexAttribute->setVertexBaseType( QAttribute::UnsignedShort );
	m_indexAttribute->setBuffer( m_indexBuffer );
	m_indexAttribute->setCount( c_indices );

	m_vertexBuffer->setDataGenerator(
		QSharedPointer< LeafVertexBufferFunctor >::create() );
	m_indexBuffer->setDataGenerator(
		QSharedPointer< LeafIndexBufferFunctor >::create() );

	q->addAttribute( m_positionsAttr );
	q->addAttribute( m_texCoordAttr );
	q->addAttribute( m_normalsAttr );
	q->addAttribute( m_indexAttribute );
}


//
// LeafGeometry
//

LeafGeometry::LeafGeometry( Qt3DCore::QNode * parent )
	:	QGeometry( parent )
	,	d( new LeafGeometryPrivate( this ) )
{
	d->init();
}

Qt3DRender::QAttribute *
LeafGeometry::positionAttribute() const
{
	return d->m_positionsAttr;
}

Qt3DRender::QAttribute *
LeafGeometry::texCoordAttribute() const
{
	return d->m_texCoordAttr;
}

Qt3DRender::QAttribute *
LeafGeometry::normalsAttribute() const
{
	return d->m_normalsAttr;
}

Qt3DRender::QAttribute *
LeafGeometry::indexiesAttribute() const
{
	return d->m_indexAttribute;
}
