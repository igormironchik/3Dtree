
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
#include "constants.hpp"

// Qt include.
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QBufferDataGenerator>

#include <QVector3D>


using namespace Qt3DRender;

namespace /* anonymous */ {

//! Number of vertexes.
static const int c_vertsNumber = 9 * 2;
//! Size of element - vec3 pos, vec3 normal.
static const quint32 c_elementSize = 3 + 3;
//! Stride.
static const quint32 c_stride = c_elementSize * sizeof( float );
//! Faces number.
static const int c_faces = 8 * 4;
//! Indicies number.
static const int c_indices = c_faces * 3;


/*
	      0 *------------x
	   1 *  |2 *
	        |
	3 *   4 *   5 *
	        |
	   6 *  |7 *
	      8 *
	        |
	        z
*/

static const float c_leafThickness = -0.01f;
//
// createLeafVertexData
//

//! Create leaf vertex data.
QByteArray createLeafVertexData()
{
	QByteArray bufferBytes;
	bufferBytes.resize( c_elementSize * c_vertsNumber * sizeof( float ) );

	float * fptr = reinterpret_cast< float* > ( bufferBytes.data() );

	const QVector3D v0( 0.0f, 0.0f, 0.0f );
	const QVector3D v9( 0.0f, 0.0f, c_leafThickness );

	const QVector3D v1( -0.25f, 0.25f, 0.0f );
	const QVector3D v10( -0.25f, 0.25f, c_leafThickness );

	const QVector3D v2( 0.25f, 0.25f, 0.0f );
	const QVector3D v11( 0.25f, 0.25f, c_leafThickness );

	const QVector3D v3( -0.5f, 0.75f, 0.0f );
	const QVector3D v12( -0.5f, 0.75f, c_leafThickness );

	const QVector3D v4( 0.0f, 0.75f, 0.0f );
	const QVector3D v13( 0.0f, 0.75f, c_leafThickness );

	const QVector3D v5( 0.5f, 0.75f, 0.0f );
	const QVector3D v14( 0.5f, 0.75f, 0.0f );

	const QVector3D v6( -0.25f, 1.25f, 0.0f );
	const QVector3D v15( -0.25f, 1.25f, c_leafThickness );

	const QVector3D v7( 0.25f, 1.25f, 0.0f );
	const QVector3D v16( 0.25f, 1.25f, c_leafThickness );

	const QVector3D v8( 0.0f, 1.5f, 0.0f );
	const QVector3D v17( 0.0f, 1.5f, c_leafThickness );

	const QVector3D topFaceNormal = QVector3D::normal( v0, v2, v1 );
	const QVector3D bottomFaceNormal = QVector3D::normal( v9, v10, v11 );

	const QVector3D n0910 = QVector3D::normal( v0, v9, v10 );
	const QVector3D n110 = QVector3D::normal( v2, v10, v12 );
	const QVector3D n312 = QVector3D::normal( v3, v12, v15 );
	const QVector3D n615 = QVector3D::normal( v6, v15, v17 );
	const QVector3D n0911 = QVector3D::normal( v11, v9, v0 );
	const QVector3D n211 = QVector3D::normal( v14, v11, v2 );
	const QVector3D n514 = QVector3D::normal( v16, v14, v5 );
	const QVector3D n716 = QVector3D::normal( v17, v16, v7 );

	const QVector3D n0 = QVector3D( topFaceNormal + n0910 + n0911 ).normalized();
	const QVector3D n9 = QVector3D( bottomFaceNormal + n0910 + n0911 ).normalized();

	const QVector3D n1 = QVector3D( topFaceNormal + n110 + n0910 ).normalized();
	const QVector3D n10 = QVector3D( bottomFaceNormal + n110 + n0910 ).normalized();

	const QVector3D n2 = QVector3D( topFaceNormal + n211 + n0911 ).normalized();
	const QVector3D n11 = QVector3D( bottomFaceNormal + n211 + n0911 ).normalized();

	const QVector3D n3 = QVector3D( topFaceNormal + n312 + n110 ).normalized();
	const QVector3D n12 = QVector3D( bottomFaceNormal + n312 + n110 ).normalized();

	const QVector3D n4 = topFaceNormal.normalized();
	const QVector3D n13 = bottomFaceNormal.normalized();

	const QVector3D n5 = QVector3D( topFaceNormal + n514 + n211 ).normalized();
	const QVector3D n14 = QVector3D( bottomFaceNormal + n514 + n211 ).normalized();

	const QVector3D n6 = QVector3D( topFaceNormal + n615 + n312 ).normalized();
	const QVector3D n15 = QVector3D( bottomFaceNormal + n615 + n312 ).normalized();

	const QVector3D n7 = QVector3D( topFaceNormal + n716 + n514 ).normalized();
	const QVector3D n16 = QVector3D( bottomFaceNormal + n716 + n514 ).normalized();

	const QVector3D n8 = QVector3D( topFaceNormal + n615 + n716 ).normalized();
	const QVector3D n17 = QVector3D( bottomFaceNormal + n615 + n716 ).normalized();

	QVector< QVector3D > vertices = QVector< QVector3D > ()
		<< v0 << n0
		<< v1 << n1
		<< v2 << n2
		<< v3 << n3
		<< v4 << n4
		<< v5 << n5
		<< v6 << n6
		<< v7 << n7
		<< v8 << n8
		<< v9 << n9
		<< v10 << n10
		<< v11 << n11
		<< v12 << n12
		<< v13 << n13
		<< v14 << n14
		<< v15 << n15
		<< v16 << n16
		<< v17 << n17;

	int idx = 0;

	for( const auto & v : qAsConst( vertices ) )
	{
		fptr[ idx++ ] = v.x();
		fptr[ idx++ ] = v.y();
		fptr[ idx++ ] = v.z();
	}

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

	// Top

	*indexPtr++ = 0;
	*indexPtr++ = 2;
	*indexPtr++ = 4;

	*indexPtr++ = 2;
	*indexPtr++ = 5;
	*indexPtr++ = 4;

	*indexPtr++ = 5;
	*indexPtr++ = 7;
	*indexPtr++ = 4;

	*indexPtr++ = 7;
	*indexPtr++ = 8;
	*indexPtr++ = 4;

	*indexPtr++ = 8;
	*indexPtr++ = 6;
	*indexPtr++ = 4;

	*indexPtr++ = 6;
	*indexPtr++ = 3;
	*indexPtr++ = 4;

	*indexPtr++ = 3;
	*indexPtr++ = 1;
	*indexPtr++ = 4;

	*indexPtr++ = 1;
	*indexPtr++ = 0;
	*indexPtr++ = 4;

	// Bottom.

	*indexPtr++ = 10;
	*indexPtr++ = 13;
	*indexPtr++ = 9;

	*indexPtr++ = 9;
	*indexPtr++ = 13;
	*indexPtr++ = 11;

	*indexPtr++ = 11;
	*indexPtr++ = 13;
	*indexPtr++ = 14;

	*indexPtr++ = 14;
	*indexPtr++ = 13;
	*indexPtr++ = 16;

	*indexPtr++ = 16;
	*indexPtr++ = 13;
	*indexPtr++ = 17;

	*indexPtr++ = 17;
	*indexPtr++ = 13;
	*indexPtr++ = 15;

	*indexPtr++ = 15;
	*indexPtr++ = 13;
	*indexPtr++ = 12;

	*indexPtr++ = 12;
	*indexPtr++ = 13;
	*indexPtr++ = 10;

	// Left side

	*indexPtr++ = 0;
	*indexPtr++ = 9;
	*indexPtr++ = 10;

	*indexPtr++ = 1;
	*indexPtr++ = 10;
	*indexPtr++ = 0;

	*indexPtr++ = 1;
	*indexPtr++ = 10;
	*indexPtr++ = 12;

	*indexPtr++ = 3;
	*indexPtr++ = 12;
	*indexPtr++ = 1;

	*indexPtr++ = 3;
	*indexPtr++ = 12;
	*indexPtr++ = 15;

	*indexPtr++ = 6;
	*indexPtr++ = 15;
	*indexPtr++ = 3;

	*indexPtr++ = 6;
	*indexPtr++ = 15;
	*indexPtr++ = 17;

	*indexPtr++ = 8;
	*indexPtr++ = 17;
	*indexPtr++ = 6;

	// Right side

	*indexPtr++ = 0;
	*indexPtr++ = 9;
	*indexPtr++ = 11;

	*indexPtr++ = 2;
	*indexPtr++ = 11;
	*indexPtr++ = 0;

	*indexPtr++ = 2;
	*indexPtr++ = 11;
	*indexPtr++ = 14;

	*indexPtr++ = 5;
	*indexPtr++ = 14;
	*indexPtr++ = 2;

	*indexPtr++ = 5;
	*indexPtr++ = 14;
	*indexPtr++ = 16;

	*indexPtr++ = 7;
	*indexPtr++ = 16;
	*indexPtr++ = 5;

	*indexPtr++ = 7;
	*indexPtr++ = 16;
	*indexPtr++ = 17;

	*indexPtr++ = 8;
	*indexPtr++ = 17;
	*indexPtr++ = 7;

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
	m_positionsAttr->setByteOffset( 0 );

	m_normalsAttr->setName( QAttribute::defaultNormalAttributeName() );
	m_normalsAttr->setVertexBaseType( QAttribute::Float );
	m_normalsAttr->setVertexSize( 3 );
	m_normalsAttr->setAttributeType( QAttribute::VertexAttribute );
	m_normalsAttr->setBuffer( m_vertexBuffer );
	m_normalsAttr->setByteStride( c_stride );
	m_normalsAttr->setByteOffset( 3 * sizeof( float ) );
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
LeafGeometry::normalsAttribute() const
{
	return d->m_normalsAttr;
}

Qt3DRender::QAttribute *
LeafGeometry::indexiesAttribute() const
{
	return d->m_indexAttribute;
}
