
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

#ifndef TREE_LEAF_MESH_HPP__INCLUDED
#define TREE_LEAF_MESH_HPP__INCLUDED

// Qt include.
#include <Qt3DRender/QGeometryRenderer>


//
// LeafMesh
//

//! Leaf mesh.
class LeafMesh Q_DECL_FINAL
	:	public Qt3DRender::QGeometryRenderer
{
	Q_OBJECT

public:
	explicit LeafMesh( Qt3DCore::QNode * parent = Q_NULLPTR );
	~LeafMesh();

private:
	void setInstanceCount( int instanceCount );
	void setVertexCount( int vertexCount );
	void setIndexOffset( int indexOffset );
	void setFirstInstance( int firstInstance );
	void setRestartIndexValue( int index );
	void setPrimitiveRestartEnabled( bool enabled );
	void setGeometry( Qt3DRender::QGeometry * geometry );
	void setPrimitiveType( PrimitiveType primitiveType );
};

#endif // TREE_LEAF_MESH_HPP__INCLUDED
