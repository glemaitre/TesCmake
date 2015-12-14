////////////////////////////////////////////////////////
/// \file frameinfo.h
/// \brief Info on a frame
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////

#ifndef SENSOR_FRAME_INFO_H
#define SENSOR_FRAME_INFO_H

#include "frame.h"
#include "typedef.h"
#include "pushpop.h"

#include <string>
#include <ostream>

namespace sensor
{
	/// \brief Frame parameters
	struct FrameInfo
	{
		enum
		{
			no_data = 0,
			n_params = 8,
			static_size = (n_params-1) * 4 + sizeof(FrameType),
		};
		FrameType type = FrameType::unknown;
		int32 width = no_data;
		int32 height = no_data;
		uint32 pixsize = no_data;
		float_t hfov = no_data;
		float_t vfov = no_data;
		int32 valmin = no_data;
		int32 valmax = no_data;

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
			if (hfov != no_data)
				count++;
			if (vfov != no_data)
				count++;
			if (pixsize != no_data)
				count++;
			if (valmin != no_data)
				count++;
			if (valmax != no_data)
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
			type = FrameType::unknown;
			width = no_data;
			height = no_data;
			pixsize = no_data;
			hfov = no_data;
			vfov = no_data;
			valmin = no_data;
			valmax = no_data;
		}
		/// \brief size of the info
		int32 size()const
		{
			return static_size;
		}
		/// \brief Default constructor
		FrameInfo(){}
		/// \brief Constructor with w/h size
		FrameInfo(int32 w, int32 h) :
			width(w), height(h){}
	};

	/// \brief Put parameters in a string
	std::string ToString(const FrameInfo& info);

	/// \brief Put parameter name in ostream
	inline std::ostream& operator<<(std::ostream& os,
		const FrameInfo& info)
	{
		return os << ToString(info) << std::endl;
	}
	/// \brief Push the info in a buffer
	inline char* Push(const FrameInfo& info, char*& buffer)
	{
		util::PushValue<>(info.type, buffer);
		util::PushValue<>(info.width, buffer);
		util::PushValue<>(info.height, buffer);
		util::PushValue<>(info.pixsize, buffer);
		util::PushValue<>(info.hfov, buffer);
		util::PushValue<>(info.vfov, buffer);
		util::PushValue<>(info.valmin, buffer);
		util::PushValue<>(info.valmax, buffer);
		return buffer;
	}
	/// \brief Pop the info out of a buffer
	inline const char* Pop(FrameInfo& info, const char*& buffer)
	{
		util::PopValue<>(info.type, buffer);
		util::PopValue<>(info.width, buffer);
		util::PopValue<>(info.height, buffer);
		util::PopValue<>(info.pixsize, buffer);
		util::PopValue<>(info.hfov, buffer);
		util::PopValue<>(info.vfov, buffer);
		util::PopValue<>(info.valmin, buffer);
		util::PopValue<>(info.valmax, buffer);
		return buffer;
	}
	inline char* Pop(FrameInfo& info, char*& buffer)
	{
		util::PopValue<>(info.type, buffer);
		util::PopValue<>(info.width, buffer);
		util::PopValue<>(info.height, buffer);
		util::PopValue<>(info.pixsize, buffer);
		util::PopValue<>(info.hfov, buffer);
		util::PopValue<>(info.vfov, buffer);
		util::PopValue<>(info.valmin, buffer);
		util::PopValue<>(info.valmax, buffer);
		return buffer;
	}
} // sensor
#endif
