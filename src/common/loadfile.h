/// \file loadfile.h
/// \brief sensor header
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date November 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr


#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include "sensor.h"
#include "core.h"
#include "typedef.h"

typedef core::Point3D<float_t> Point3D;
typedef core::StructuredPointCloud<Point3D> PointCloud;

bool LoadFile(const char* name,PointCloud& pts)
{
	const char* map_name = "depth2world.bin";
	bool res = true;
	// Depth Reader
	sensor::IOFrame<uint16> ioframe;
	// Depth frame
	sensor::DepthFrame frame;
	// Frame -> Points
	sensor::Mapper mapper;
	// info 
	sensor::FrameInfo info;

	mapper.init(map_name);
	// Set filename
	ioframe.filename(name);
	ioframe.read_info(info);
	if (ioframe.read(frame))
		res = mapper.point3d(frame, pts);
	return res;
}
#endif