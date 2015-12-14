
// mapper.cpp
#include "mapper.h"
#include "iofile.h"

namespace sensor
{
	// Write info
	bool Mapper::write_info_()
	{
		MapInfo info;
		info.width = map_.size_y();
		info.height = map_.size_x();
		info.pixsize = sizeof(Point2D);
		info.type = MapType::depth;

		const uint32 buffer_size = MapInfo::static_size;
		const char* buffer = reinterpret_cast<const char*>(&info);
		return interface_.write(buffer, buffer_size);
	}
	// Read info
	bool Mapper::read_info_(MapInfo& info)
	{
		const uint32 buffer_size = MapInfo::static_size;
		char* buffer = reinterpret_cast<char*>(&info);
		bool res = false;
		if (interface_.read(buffer, buffer_size))
			res = IsValid(info.type);
		return res;
	}
	// Write data
	bool Mapper::write(const std::string& name)
	{
		interface_.filename(name);
		bool res = false;
		if (write_info_())
		{
			uint32 map_size = map_.size();
			uint32 buffer_size = map_size * sizeof(Point2D);

			const char* buffer = reinterpret_cast<const char*>(map_.data());
			res =  interface_.write(buffer,buffer_size);
		}
		return res;
	}

	// Read data
	bool Mapper::read(const std::string& name)
	{
		interface_.filename(name);
		MapInfo info;
		bool res = false;
		if (read_info_(info))
		{
			int32 buffer_size = info.height * info.width * sizeof(Point2D);
			map_.create(info.height, info.width);
			char* buffer = reinterpret_cast<char*>(map_.data());
			res = interface_.read(buffer, buffer_size) == buffer_size;
		}
		return res;
	}
	/// Convert depth point to 3D point
	bool Mapper::point3d(const Point3D& pti, Point3D& pto)const
	{
		const float_t fz = 0.001f;
		bool res = false;
		uint32 ii = uint32(pti[1]);
		uint32 jj = uint32(pti[0]);
		float_t z = float_t(pti[2])* fz;
		if (ii < map_.size_x() && map_.size_y())
		{
			const auto& xy = map_[ii][jj];
			pto[0] = xy[0] * z;
			pto[1] = xy[1] * z;
			pto[2] = z;
			res = true;
		}
		return res;
	}

} // sensor
