////////////////////////////////////////////////////////
/// \file mapper.h
/// \brief Coordinate Mapper between frame
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////
#ifndef SENSOR_MAPPER_H
#define SENSOR_MAPPER_H

#include "frame.h"
#include "iofile.h"
#include "sensortypedef.h"
#include "core.h"
#include <string>

namespace sensor
{
	/// \brief Map type
	enum struct MapType
	{
		depth = 0x0B0B0B0B,
		unknown,
		error,
	};
	inline bool IsValid(const MapType& type)
	{
		return type < MapType::error &&
			type >= MapType::depth;
	}

	inline bool IsKnown(const MapType& type)
	{
		return type < MapType::unknown &&
			type >= MapType::depth;
	}
	/// \brief Info about a map
	struct MapInfo
	{
		enum
		{
			no_data = 0,
			n_params = 4,
			static_size = (n_params - 1) * 4 + sizeof(MapType),
		};

		MapType type = MapType::unknown;
		int32 width = no_data;
		int32 height = no_data;
		uint32 pixsize = no_data;

		/// \brief Count the number of valid parameters
		int32 count_valid()const
		{
			int32 count = 0;
			if (IsKnown(type))
				count++;
			if (width != no_data)
				count++;
			if (height != no_data)
				count++;
			if (pixsize != no_data)
				count++;
			return count;
		}

		/// \brief Are all paramters valid?
		bool is_complete()const
		{
			return count_valid() == n_params;
		}
		/// \brief Set all paramters to no data
		void init()
		{
			type = MapType::unknown;
			width = no_data;
			height = no_data;
			pixsize = no_data;
		}
		/// \brief size of the info
		int32 size()const
		{
			return static_size;
		}
		/// \brief Default constructor
		MapInfo(){}
		/// \brief Constructor with w/h size
		MapInfo(int32 w, int32 h) :
			width(w), height(h){}

	};

	/// \brief
	class Mapper
	{
	public:
		typedef core::Matrix<Point2D> map_type;
		typedef DepthFrame::container_type buffer_type;
	private:
		map_type map_;
		IOFile interface_;
		// Read informations
		bool read_info_(MapInfo& info);
		// Write informations
		bool write_info_();

	public:
		typedef DepthFrame::container_type depth_buffer_t;

		bool init(const std::string& name = "depth2world.bin")
		{
			return read(name);
		}
        bool init(const std::wstring& wname)
        {
            return read(wname);
        }

		/// \brief Convert a depth frame to point clouds
		template<class Pts>
		bool point3d(const DepthFrame& frame, Pts& pts)const;
		/// \brief Convert a depth buffer to point clouds
		template<class Pts>
		bool point3d(const buffer_type& buffer, Pts& pts)const;
		/// \brief Specialization for structured point cloud
		template<class Pt>
		bool point3d(const buffer_type& buffer, 
			core::StructuredPointCloud<Pt>& pts) const;
		/// \brief Convert depth point to 3D point
		template<class Pts>
		bool point3d(const Pts& pts_in, Pts& pts_out)const;
		/// \brief Convert depth point to 3D point
		bool point3d(const Point3D& pt, Point3D&)const;
		/// \brief Check the validity of the measurements
		template<class Pts>
		static uint32 check(Pts& pts);
		static bool check(Point3D& pt);

		// Get info
		bool info(MapInfo& minfo)const;

		// Read calibration parameters
		bool read(const std::string& name);

		bool read(const std::wstring& wname)
		{
            return read(std::string(wname.begin(), wname.end()));
		}
		// Write calibration parameters
		bool write(const std::string& name);

		bool write(const std::wstring& wname)
		{
            return write(std::string(wname.begin(), wname.end()));
		}
		// Const access to the map
		const map_type& map()const
		{
			return map_;
		}
		// Access to the map
		map_type& map()
		{
			return map_;
		}
		void map(const map_type& map,bool clone = false)
		{
			if (clone)
				core::Clone(map, map_);
			else
				map_ = map;
		}
	};

	// Convert a depth frame to point clouds
	template<class Pts>
	inline bool Mapper::point3d(const DepthFrame& frame, Pts& pts)const
	{
		bool res = point3d(frame.container, pts);
		pts.timestamp(frame.timestamp);
		return res;
	}

	template<class Pts>
	bool Mapper::point3d(const depth_buffer_t& buffer, Pts& pts)const
	{
		const uint32 n_pts = buffer.size();
		const uint32 sx = map_.size_x();
		const uint32 sy = map_.size_y();
		const float_t fz = 0.001f;
		bool  res = false;
		if (n_pts == map_.size())
		{
			pts.resize(n_pts);
			uint32 count = 0;
			for (uint32 i = 0; i < sx; ++i)
				for (uint32 j = 0; j < sy;++j)
				{
					auto& pt = pts[count];
					const auto& xy = map_[i][j];

					float_t z = float_t(buffer[count])* fz;

					pt[0] = xy[0] * z;
					pt[1] = xy[1] * z;
					pt[2] = z;
					++count;
				}
			res = true;
		}
		return res;
	}
	/// Point3d
	template<typename Pt>
	bool Mapper::point3d(const buffer_type& buffer,
		core::StructuredPointCloud<Pt>& pts) const
	{
		const uint32 n_pts = buffer.size();
		const uint32 sx = map_.size_x();
		const uint32 sy = map_.size_y();
		const float_t fz = 0.001f;
		bool  res = false;
		if (n_pts == map_.size())
		{
			pts.create(sx, sy);
			uint32 count = 0;
			for (uint32 i = 0; i < sx; ++i)
				for (uint32 j = 0; j < sy; ++j)
				{
					auto& pt = pts[i][j];
					const auto& xy = map_[i][j];

					float_t z = float_t(buffer[count])* fz;

					pt[0] = xy[0] * z;
					pt[1] = xy[1] * z;
					pt[2] = z;
					++count;
				}
			res = true;
		}
		return res;
	}
	/// \brief Convert depth point to 3D point
	template<class Pts>
	bool Mapper::point3d(const Pts& pts_in, Pts& pts_out)const
	{
		const uint32 n_pts = pts_in.size();
		pts_out.resize(n_pts);
		const float_t fz = 0.001f;
		bool  res = false;

		if (map_.size() > 0)
		{
			res = true;
			uint32 count = 0;
			for (uint32 i = 0; i < n_pts;++i)
			{
				const auto& pti = pts_in[i];
				auto& pto = pts_out[i];
				uint32 ii = uint32(pti[1]);
				uint32 jj = uint32(pti[0]);
				float_t z = float_t(pti[2])* fz;
				const auto& xy = map_[ii][jj];

				pto[0] = xy[0] * z;
				pto[1] = xy[1] * z;
				pto[2] = z;
			}
		}
		return res;
	}
	/// Check the validity of the measurements
	template<class Pts>
	uint32 Mapper::check(Pts& pts)
	{
		uint32 count = 0;
		for (auto& pt : pts)
			if (check(pt))
				count++;
		return count;
	}
	inline bool Mapper::check(Point3D& pt)
	{
		bool res = pt[2] > 0;
		if (!res)
			pt.invalidate();
		return res;
	}
} // sensor
#endif
