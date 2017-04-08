
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

#ifndef TREE_LEAF_GEOMETRY_HPP__INCLUDED
#define TREE_LEAF_GEOMETRY_HPP__INCLUDED

// Qt include.
#include <Qt3DRender/QGeometry>

#include <QScopedPointer>


//
// LeafGeometry
//

class LeafGeometryPrivate;

//! Geometry of the leaf.
class LeafGeometry Q_DECL_FINAL
	:	public Qt3DRender::QGeometry
{
public:
	LeafGeometry( Qt3DCore::QNode * parent );

	//! \return Position attribute.
	Qt3DRender::QAttribute * positionAttribute() const;

	//! \return Normals attribute.
	Qt3DRender::QAttribute * normalsAttribute() const;

	//! \return Indexies attribute.
	Qt3DRender::QAttribute * indexiesAttribute() const;

private:
	Q_DISABLE_COPY( LeafGeometry )

	QScopedPointer< LeafGeometryPrivate > d;
}; // class LeafGeometry

#endif // TREE_LEAF_GEOMETRY_HPP__INCLUDED
