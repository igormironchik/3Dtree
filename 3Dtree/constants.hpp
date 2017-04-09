
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

#ifndef TREE__CONSTANTS_HPP__INCLUDED
#define TREE__CONSTANTS_HPP__INCLUDED

// Qt include.
#include <QtGlobal>


//! Degries in radian.
static const float c_degInRad = 57.2958f;


//
// Leaf constants.
//

//! Base scale for the leaf, i.e. when age is 1.0.
static const float c_leafBaseScale = 1.5f;
//! Base angle for the leaf plain to the orthogonal plain of the branch.
static const float c_leafAngle = 90.0f;
//! Leaf rotation distortion.
static const float c_leafRotationDistortion = 50.0f;
//! Positive value of leaf vertex distortion.
static const float c_positiveLeafDistortion = 0.05f;
//! Negative value of leaf vertex distortion.
static const float c_negativeLeafDistortion = - c_positiveLeafDistortion;
//! Count of leafs.
static const quint8 c_leafsCount = 2;


//
// Branch constants.
//

//! Distortion of the radius of branch.
static const float c_branchDistortion = 0.25f;
//! Delta of the branch radius.
static const float c_branchRadiusDelta = 0.3f;
//! Start branch radius.
static const float c_startBranchRadius = 0.015f;
//! Branch length.
static const float c_branchLength = 1.25f;
//! Distortion of the branch length.
static const float c_branchLengthDistortion = 0.5f;
//! Max angle of the child branch.
static const float c_maxBranchAngle = 35.0f;
//! Count of child branches.
static const quint8 c_childBranchesCount = 4;
//! Should branch has continuation (parallel to parent) branch?
static const bool c_hasContinuationBranch = true;
//! Rotation of the branch distortion.
static const float c_branchRotationDistortion = 35.0f;

#endif // TREE__CONSTANTS_HPP__INCLUDED
