
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


//
// Leaf constants.
//

//! Base scale for the leaf, i.e. when age is 1.0.
static const float c_leafBaseScale = 0.25f;
//! Base angle for the leaf plain to the orthogonal plain of the branch.
static const float c_leafAngle = 45.0f;
//! Leaf rotation distortion.
static const float c_leafRotationDistortion = 50.0f;
//! Count of leafs.
static const quint8 c_leafsCount = 2;


//
// Branch constants.
//

//! Distortion of the radius of branch.
static const float c_branchDistortion = 0.01f;
//! Delta of the branch radius.
static const float c_branchRadiusDelta = 0.003f;
//! Start branch radius.
static const float c_startBranchRadius = 0.035f;
//! Branch length.
static const float c_branchLength = 0.35f;
//! Distortion of the branch length.
static const float c_branchLengthDistortion = 0.15f;
//! Max angle of the child branch.
static const float c_maxBranchAngle = 55.0f;
//! Count of child branches. Can't be less than 1.
static const quint8 c_childBranchesCount = 4;
//! Should branch has continuation (parallel to parent) branch?
static const bool c_hasContinuationBranch = true;
//! Rotation of the branch distortion.
static const float c_branchRotationDistortion = 35.0f;
//! How much branch should be longer after 100 years.
static const float c_branchLengthMultiplicator = 50.0f;
//! How slower should branch grow then tree.
static const float c_branchSlower = 2.0f;
//! How much branch should be scaled after 100 years.
static const float c_branchScale = 5.0f;

#endif // TREE__CONSTANTS_HPP__INCLUDED
