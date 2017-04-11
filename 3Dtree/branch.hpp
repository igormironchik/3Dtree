
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

#ifndef TREE__BRANCH_HPP__INCLUDED
#define TREE__BRANCH_HPP__INCLUDED

// Qt include.
#include <Qt3DCore/QEntity>
#include <QScopedPointer>
#include <Qt3DCore/QTransform>

QT_BEGIN_NAMESPACE

namespace Qt3DExtras {
	class QPhongMaterial;
}

QT_END_NAMESPACE


class LeafMesh;


//
// Branch
//

class BranchPrivate;

//! Branch on the tree.
class Branch Q_DECL_FINAL
	:	public Qt3DCore::QEntity
{
public:
	Branch( const QVector3D & startParentPos,
		const QVector3D & endParentPos,
		float parentRadius, bool continuation, bool isTree,
		Qt3DExtras::QPhongMaterial * material,
		LeafMesh * leafMesh,
		Qt3DCore::QEntity * parent = Q_NULLPTR );
	~Branch();

	//! Rotate on top of the parent.
	void rotate( float angle );

	//! Set age of the branch. 1.0f = 1 year, 2.0f = 2 years, and so on.
	void setAge( float age );

	//! Update position.
	void updatePosition();

	//! \return Start pos.
	const QVector3D & startPos() const;

	//! \return End pos.
	const QVector3D & endPos() const;

	//! \return Top radius.
	float topRadius() const;

	//! \return Length.
	float length() const;

protected:
	//! Place leafs.
	void placeLeafs();

private:
	friend class MainWindowPrivate;
	friend class BranchPrivate;

	Q_DISABLE_COPY( Branch )

	QScopedPointer< BranchPrivate > d;
}; // class Branch

#endif // TREE__BRANCH_HPP__INCLUDED
