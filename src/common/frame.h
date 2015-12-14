////////////////////////////////////////////////////////
/// \file frame.h
/// \brief Frame retrieved from a kinect
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////
#ifndef SENSOR_FRAME_H
#define SENSOR_FRAME_H

#include "typedef.h"
#include "pushpop.h"
#include <vector>

namespace sensor
{
	enum struct FrameType
	{
		depth = 0x0A0A0A0A,
		ir,
		color,
		points,
		unknown,
		error,
	};
    inline bool IsValid(const FrameType& type)
    {
        return type < FrameType::error &&
            type >= FrameType::depth;
    }

	inline bool IsKnown(const FrameType& type)
	{
		return type < FrameType::unknown &&
			type >= FrameType::depth;
	}

	inline std::ostream& operator<<(std::ostream& os,
		const FrameType& type)
	{
		switch(type)
		{
			case FrameType::depth :
				return os << " Depth";
			case FrameType::ir :
				return os << " IR";
			case FrameType::color :
				return os << " Color";
			case FrameType::points:
				return os << " Points 3D";
			case FrameType::unknown :
				return os << " Unknown";
			default:
				return os << " Error";
		}
	}
	/// Frame header
	struct FrameHeader
	{
		enum
		{
			static_size = sizeof(int64) + sizeof(FrameType),
			no_data = 0,
		};
		/// Time stamp
		int64 timestamp = no_data;
		/// Type
		FrameType type = FrameType::unknown;
		/// Constructor
		FrameHeader(FrameType ftype = FrameType::unknown) :
			type(ftype)
		{}
		/// Reset
		void init()
		{
			type = FrameType::unknown;
			timestamp = no_data;
		}
	};
	/// Template for frames
	template<typename T>
	struct Frame : FrameHeader
	{
		typedef T value_type;
		typedef FrameHeader header_type;
		/// Container type
		typedef std::vector<T> container_type;
		/// Data container
		container_type container;
		/// Constructor
		Frame(FrameType ftype = FrameType::unknown) :
			FrameHeader(ftype)
		{}
		/// Size
		uint32 size()const
		{
			return container.size();
		}
		/// Is empty?
		bool empty()const
		{
			return container.empty();
		}
		/// Access operator
		const T& operator[](uint32 ind)const
		{
			return container[ind];
		}
		/// Assignment operator
		T& operator[](uint32 ind)
		{
			return container[ind];
		}
		/// Access to pointer
		T* data()
		{
			return container.data();
		}
		const T* data()const
		{
			return container.data();
		}

	};
	/// Depth frame
	struct DepthFrame : public Frame < uint16 >
	{
		/// \brief Constructor
		DepthFrame() :
			Frame<uint16>(FrameType::depth)
		{}
	};
	// Ir frame
	struct IrFrame : public Frame < uint16 >
	{
		/// \brief Constructor
		IrFrame() :
			Frame<uint16>(FrameType::ir)
		{}
	};

	// Color frame
	struct ColorFrame : public Frame <uint8>
	{
		/// \brief Constructor
		ColorFrame():
			Frame<uint8>(FrameType::color)
		{}
	};

	/// \brief Push the info in a buffer
	inline char* Push(const FrameHeader& header, char*& buffer)
	{
		util::PushValue<>(header.type, buffer);
		util::PushValue<>(header.timestamp, buffer);
		return buffer;
	}
	/// \brief Pop the info out of a buffer
	inline const char* Pop(FrameHeader& header, const char*& buffer)
	{
		util::PopValue<>(header.type, buffer);
		util::PopValue<>(header.timestamp, buffer);
		return buffer;
	}
	inline char* Pop(FrameHeader& header, char*& buffer)
	{
		util::PopValue<>(header.type, buffer);
		util::PopValue<>(header.timestamp, buffer);
		return buffer;
	}

	template<typename T,typename U = T>
	struct Sampler
	{
		static void sub(std::vector<T>& buffer,
			uint32 s,uint32 stride = 0);
		static void sub(const std::vector<T>& src,
			std::vector<T>& dst,
			uint32 s, uint32 stride = 0);

		static void over(std::vector<T>& buffer,
			uint32 s, uint32 stride = 0);
		static void over(const std::vector<T>& src,
			std::vector<T>& dst,
			uint32 s, uint32 stride = 0);

	};

	/// \brief In place subsampling
	template<typename T,typename U>
	void Sampler<T, U>::sub(std::vector<T>& buffer,
		uint32 s, uint32 stride)
	{
		if (stride == 0)
			stride = buffer.size() * sizeof(T);

		uint32 width_t = stride / sizeof(T);
		uint32 heigth_t = buffer.size() / width_t;
		uint32 newsize_t = (width_t / s)*(heigth_t / s);

		uint32 width_u = stride / sizeof(U);
		uint32 size_u = (buffer.size()* sizeof(T)) / sizeof(U);

		const U* in = reinterpret_cast<const U*>(buffer.data());
		const U* end = in + size_u;
		U* out = reinterpret_cast<U*>(buffer.data());

		while (in < end)
		{
			const U* endline = in + width_u;
			while (in < endline)
			{
				*out = *in;
				out++;
				in += s;
			}
			in = endline;
			in += (width_u * (s - 1));
		}

		buffer.resize(newsize_t);
	}
	/// \brief In place subsampling
	template<typename T, typename U>
	void Sampler<T, U>::sub(const std::vector<T>& src,
		std::vector<T>& dst,
		uint32 s, uint32 stride)
	{
		if (stride == 0)
			stride = src.size();

		uint32 width_t = stride / sizeof(T);
		uint32 heigth_t = src.size() / width_t;
		uint32 newsize_t = (width_t / s)*(heigth_t / s);
		dst.resize(newsize_t);

		uint32 width_u = stride / sizeof(U);
		uint32 size_u = (src.size()* sizeof(T)) / sizeof(U);

		const U* in = reinterpret_cast<const U*>(src.data());
		const U* end = in + size_u;
		U* out = reinterpret_cast<U*>(dst.data());

		while (in < end)
		{
			const U* endline = in + width_u;
			while (in < endline)
			{
				*out = *in;
				out++;
				in += s;
			}
			in = endline;
			in += (width_u * (s - 1));
		}
	}

	/// \brief In place oversampling
	template<typename T, typename U>
	void Sampler<T, U>::over(std::vector<T>& buffer,
		uint32 s, uint32 stride)
	{
		if (stride == 0)
			stride = buffer.size() * sizeof(T);

		uint32 width_t = stride / sizeof(T);
		uint32 heigth_t = buffer.size() / width_t;
		uint32 newsize_t = (width_t * s)*(heigth_t * s);

		uint32 width_u = stride / sizeof(U);
		uint32 size_u = (buffer.size()* sizeof(T)) / sizeof(U);

		buffer.resize(newsize_t);

		const U* begin = reinterpret_cast<const U*>(buffer.data());
		const U* in = begin + size_u;

		U* out = reinterpret_cast<U*>(buffer.data());
		out += size_u*s*s;

		while (in > begin)
		{
			const U* beginline = in - width_u;
			while (in > beginline)
			{
				--in;
				for (uint32 i = 0; i < s; ++i)
					*(--out) = *in;
			}
			beginline = out;
			for (uint32 i = 1; i < s; ++i)
			{
				out -= width_u * s;
				memcpy(out,beginline, width_u * s * sizeof(U));
			}
		}
	}

	/// \brief Oversampling
	template<typename T, typename U>
	void Sampler<T, U>::over(const std::vector<T>& src,
		std::vector<T>& dst, uint32 s, uint32 stride)
	{
		if (stride == 0)
			stride = src.size()*sizeof(T);

		uint32 width_t = stride / sizeof(T);
		uint32 heigth_t = src.size() / width_t;
		uint32 newsize_t = (width_t * s)*(heigth_t * s);

		uint32 width_u = stride / sizeof(U);
		uint32 size_u = (src.size()* sizeof(T)) / sizeof(U);

		dst.resize(newsize_t);

		const U* begin = reinterpret_cast<const U*>(src.data());
		const U* in = begin + size_u;
		U* out = reinterpret_cast<U*>(dst.data()); +size_u*s*s;
		while (in > begin)
		{
			const U* beginline = in - width_u;
			while (in < beginline)
			{
				--in;
				for (uint32 i = 0; i < s; ++i)
					*(--out) = *in;
			}
			beginline = out;
			for (uint32 i = 1; i < s; ++i)
			{
				out -= width_u * s;
				memcpy(out, beginline, width_u * s*sizeof(U));
			}
		}
	}
} // sensor

#endif
