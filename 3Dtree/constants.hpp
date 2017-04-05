
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

//! Base scale for the leaf, i.e. when age is 1.0.
static const float c_leafBaseScale = 0.1f;
//! Degries in radian.
static const float c_degInRad = 57.2958f;
//! Base angle for the leaf plain to the orthogonal plain of the branch.
static const float c_leafAngle = 35.0f;
//! Negative value of leaf vertex distortion.
static const float c_negativeLeafDistortion = -0.05f;
//! Positive value of leaf vertex distortion.
static const float c_positiveLeafDistortion = 0.05f;

#endif // TREE__CONSTANTS_HPP__INCLUDED
