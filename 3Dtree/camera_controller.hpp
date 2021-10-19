
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

#ifndef TREE__CAMERA_CONTROLLER_HPP__INCLUDED
#define TREE__CAMERA_CONTROLLER_HPP__INCLUDED

// Qt include.
#include <Qt3DCore/QEntity>

// C++ include.
#include <memory>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
	class QCamera;
}

QT_END_NAMESPACE


//
// CameraController
//

class CameraControllerPrivate;

class CameraController Q_DECL_FINAL
	:	public Qt3DCore::QEntity
{
	Q_OBJECT

public:
	CameraController( Qt3DRender::QCamera * camera,
		Qt3DCore::QEntity * parent );
	~CameraController();

private slots:
	void _q_onTriggered( float );

private:
	friend class CameraControllerPrivate;

	Q_DISABLE_COPY( CameraController )

	std::unique_ptr< CameraControllerPrivate > d;
}; // class CameraController

#endif // TREE__CAMERA_CONTROLLER_HPP__INCLUDED
