
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
#include "camera_controller.hpp"

// Qt include.
#include <Qt3DRender/QCamera>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QButtonAxisInput>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QLogicalDevice>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QMouseEvent>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DCore/QTransform>


//
// CameraControllerPrivate
//

class CameraControllerPrivate {
public:
	CameraControllerPrivate( Qt3DRender::QCamera * camera,
		CameraController * parent )
		:	m_camera( camera )
		,	m_leftMouseButtonAction( new Qt3DInput::QAction( parent ) )
		,	m_rxAxis( new Qt3DInput::QAxis( parent ) )
		,	m_tzAxis( new Qt3DInput::QAxis( parent ) )
		,	m_leftMouseButtonInput( new Qt3DInput::QActionInput( parent ) )
		,	m_mouseRxInput( new Qt3DInput::QAnalogAxisInput( parent ) )
		,	m_mouseTzYInput( new Qt3DInput::QAnalogAxisInput( parent ) )
		,	m_mouseDevice( new Qt3DInput::QMouseDevice( parent ) )
		,	m_logicalDevice( new Qt3DInput::QLogicalDevice )
		,	m_frameAction( new Qt3DLogic::QFrameAction )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Camera.
	Qt3DRender::QCamera * m_camera;
	//! Left mouse button action.
	Qt3DInput::QAction * m_leftMouseButtonAction;
	//! X axis.
	Qt3DInput::QAxis * m_rxAxis;
	//! Wheel Z axis.
	Qt3DInput::QAxis * m_tzAxis;
	//! Left mouse button input.
	Qt3DInput::QActionInput * m_leftMouseButtonInput;
	//! X analog axis input.
	Qt3DInput::QAnalogAxisInput * m_mouseRxInput;
	//! Mouse Y wheel input.
	Qt3DInput::QAnalogAxisInput * m_mouseTzYInput;
	//! Mouse device.
	Qt3DInput::QMouseDevice * m_mouseDevice;
	//! Logical device.
	std::unique_ptr< Qt3DInput::QLogicalDevice > m_logicalDevice;
	//! Frame action.
	std::unique_ptr< Qt3DLogic::QFrameAction > m_frameAction;
	//! Parent.
	CameraController * q;
}; // class CameraControllerPrivate

void
CameraControllerPrivate::init()
{
	// Left Mouse Button Action
	m_leftMouseButtonInput->setButtons( QVector<int>() << Qt::LeftButton );
	m_leftMouseButtonInput->setSourceDevice( m_mouseDevice );
	m_leftMouseButtonAction->addInput( m_leftMouseButtonInput );

	// Mouse X
	m_mouseRxInput->setAxis( Qt3DInput::QMouseDevice::X );
	m_mouseRxInput->setSourceDevice( m_mouseDevice );
	m_rxAxis->addInput( m_mouseRxInput );

	m_mouseTzYInput->setAxis( Qt3DInput::QMouseDevice::WheelY );
    m_mouseTzYInput->setSourceDevice( m_mouseDevice );
    m_tzAxis->addInput( m_mouseTzYInput );

	m_logicalDevice->addAction( m_leftMouseButtonAction );
	m_logicalDevice->addAxis( m_rxAxis );
	m_logicalDevice->addAxis( m_tzAxis );

	QObject::connect( m_frameAction.get(), &Qt3DLogic::QFrameAction::triggered,
		q, &CameraController::_q_onTriggered );

	q->addComponent( m_frameAction.release() );
	q->addComponent( m_logicalDevice.release() );
}


//
// CameraController
//

CameraController::CameraController( Qt3DRender::QCamera * camera,
	Qt3DCore::QEntity * parent )
	:	Qt3DCore::QEntity( parent )
	,	d( new CameraControllerPrivate( camera, this ) )
{
	d->init();
}

CameraController::~CameraController()
{
}

void
CameraController::_q_onTriggered( float )
{
	if( d->m_leftMouseButtonAction->isActive() )
	{
		d->m_camera->rotateAboutViewCenter(
			Qt3DCore::QTransform::fromAxisAndAngle( 0.0f, 1.0f, 0.0f,
				- d->m_rxAxis->value() ) );
	}
	else
	{
		const float z = d->m_tzAxis->value();

		if( d->m_camera->position().length() > 15.0 || z < 0.0f )
			d->m_camera->translate( QVector3D( 0.0f, 0.0f, z ),
				Qt3DRender::QCamera::DontTranslateViewCenter );
	}
}
