
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

#ifndef TREE__LEAF_HPP__INCLUDED
#define TREE__LEAF_HPP__INCLUDED

// Qt include.
#include <Qt3DCore/QEntity>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

class QColor;

namespace Qt3DRender {
	class QMesh;
}

QT_END_NAMESPACE


//
// Leaf
//

class LeafPrivate;

//! Leaf on the tree.
class Leaf Q_DECL_FINAL
	:	public Qt3DCore::QEntity
{
public:
	Leaf( const QVector3D & startBranchPos,
		const QVector3D & endBranchPos,
		Qt3DRender::QMesh * mesh,
		Qt3DCore::QNode * parent = Q_NULLPTR );
	~Leaf();

	//! Set color.
	void setColor( const QColor & c );

	//! Set age in range from 0.0 to 1.0.
	void setAge( float age );

	//! Update position of the leaf.
	void updatePosition();

	//! Rotate leaf. \note The leaf is rotated around the branch with
	//! small random angle to the orthogonal plane to the branch.
	//! \par angle is angle on the orthogonal to the branch plane with
	//! center on the end of the branch.
	void rotate( float angle );

	//! \return Autumn's color.
	static QColor autumnColor();

	//! Animate fall of the leaf.
	void fallAndDie();

private:
	friend class LeafPrivate;

	Q_DISABLE_COPY( Leaf )

	QScopedPointer< LeafPrivate > d;
}; // class Leaf

#endif // TREE__LEAF_HPP__INCLUDED
