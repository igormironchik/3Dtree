
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
#include "leaf_mesh.hpp"
#include "leaf_geometry.hpp"


//
// LeafMesh
//

LeafMesh::LeafMesh( Qt3DCore::QNode * parent )
	:	Qt3DRender::QGeometryRenderer( parent )
{
	setPrimitiveType( Qt3DRender::QGeometryRenderer::Triangles );
	setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

	LeafGeometry * geometry = new LeafGeometry( this );

	QGeometryRenderer::setGeometry( geometry );

	//setVertexCount( 3 );
}

LeafMesh::~LeafMesh()
{
}

//void
//LeafMesh::setPrimitiveType( PrimitiveType primitiveType )
//{
//	Qt3DRender::QGeometryRenderer::setPrimitiveType( primitiveType );
//}

//void
//LeafMesh::setVertexCount( int vertexCount )
//{
//	Qt3DRender::QGeometryRenderer::setVertexCount( vertexCount );
//}
