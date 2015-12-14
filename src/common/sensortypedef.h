/// \file sensortypedef.h
/// \brief Definitions specific to sensors
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef SENSOR_TYPEDEF_H
#define SENSOR_TYPEDEF_H

#include "core.h"

namespace sensor
{
	/// \brief Point3D
	typedef core::Point3D<float_t> Point3D;
	/// \brief Point2D
	typedef core::Point2D<float_t> Point2D;
	/// \brief Structured point cloud
	typedef core::StructuredPointCloud<Point3D> 
		StructuredPointCloud;
} // sensor
#endif
