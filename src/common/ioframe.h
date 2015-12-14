////////////////////////////////////////////////////////
/// \file ioframe.h
/// \brief Interface to read and write frame in a file
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////

#ifndef SENSOR_IO_FRAME_H
#define SENSOR_IO_FRAME_H

#include "iofile.h"
#include "frame.h"
#include "frameinfo.h"
#include "typedef.h"

namespace sensor
{
template<typename T>
using Buffer = std::vector<T>;

/// \brief Frame interface
template<typename T,class Interface = IOFile>
class IOFrame: public Interface
{
public:
	typedef T data_type;
	typedef FrameHeader header_type;
protected:
	// Frame info
	FrameInfo info_;

	// Scale factor
	uint32 scale_ = 1;
	// Read data
	template<class Data>
	bool read_(Data& data);
	// Write data
	template<class Data>
	bool write_(const Data& data);
	// Size of a frame (without header)
	uint32 frame_size_()const;

public:
	/// \brief Constructor
	explicit IOFrame(std::string& filename, int32 width = 0, int32 height = 0) :
        Interface(filename),
		info_(width, height)
	{
		info_.pixsize = sizeof(T);
	}
	/// \brief Default constructor
	explicit IOFrame(int32 width = 0, int32 height = 0) :
		info_(width, height)
	{
		info_.pixsize = sizeof(T);
	}
	// Get scale factor
	uint32 scale()const
	{
		return scale_;
	}
	// Set scale factor
	void scale(uint32 s)
	{
		scale_ = s;
	}

	// read info
	bool read_info()
	{
		FrameInfo info;
		return read_info(info);
	}
	bool read_info(FrameInfo& info);
	// read header
	bool read_header(header_type& header)
	{
		return read_(header);
	}
	// read next frame
	template<typename U>
	bool read(Frame<U>& frame)
	{
		return read(frame.container, frame);
	}
	// read next frame in a container
	template<typename U>
	bool read(Buffer<U>& buffer,bool force = false)
	{
		header_type header;
		return read(buffer, header,force);
	}
	// read next frame in a buffer
	template<typename U>
	bool read(Buffer<U>& buffer, header_type& header,
		bool force = false);
	// write info
	bool write_info(const FrameInfo& info)
	{
		info_ = info;
		return write_info();
	}
	bool write_info();
	// write header
	bool write_header(const header_type& header)
	{
		return write_(header);
	}
	// write data
	template<typename U>
	bool write(const Frame<U>& frame)
	{
		return write(frame.container, frame);
	}
	template<typename U>
	bool write(const Buffer<U>& buffer)
	{
		header_type header;
		return write(buffer, header);
	}
	// read next frame in a buffer
	template<typename U>
	bool write(const Buffer<U>& buffer,
		const header_type& header);
	// write info
	bool write(const FrameInfo& info)
	{
		return write_info(info);
	}
	// write header
	bool write(const FrameHeader& header)
	{
		return write_header(header);
	}
	/// \brief Get the width of frames
	int32 width()const
	{
		return info_.width;
	}
	/// \brief Set the width of frames
	void width(int32 val)
	{
		info_.width = val;
	}
	/// \brief Get the height of frames
	int32 height()const
	{
		return info_.height;
	}
	/// \brief Set the height of frames
	void height(int32 val)
	{
		info_.height = val;
	}
	/// \brief Get the file info
	const FrameInfo& info()const
	{
		return info_;
	}
	FrameInfo& info()
	{
		return info_;
	}
	/// \brief Set the file info
	void info(const FrameInfo& info)
	{
		info_ = info;
	}
	/// \brief  Get the current index
	int32 get_index();

	/// \brief  Change the index of the next frame to read
	bool set_index(int32 count,bool relative = false);

	/// \brief  Count the number of frames
	int32 count();
};
////////////////////////////////////////////////////////

template<typename T, class Interface>
template<class Data>
inline bool IOFrame<T, Interface>::read_(Data& data)
{
	bool res = true;
	const int32 siz = Data::static_size;
	char buffer[siz];

	int32 pos = Interface::get_pos();
	Interface::read(buffer, siz);
	char* pchar = buffer;
	Pop(data, pchar);
	if (!IsValid(data.type))
	{
		Interface::set_pos(pos);
		res = false;
	}
	return res;
}
template<typename T, class Interface>
template<class Data>
inline bool IOFrame<T, Interface>::write_(const Data& data)
{
	const int32 siz = Data::static_size;
	char buffer[siz];
	char* p_char = buffer;
	Push(data, p_char);
	return Interface::write(buffer, siz);
}
template<typename T, class Interface>
inline bool IOFrame<T, Interface>::write_info()
{
	bool res = false;
	if (scale_ > 1)
	{
		FrameInfo info = info_;
		info.width /= scale_;
		info.height /= scale_;
		res = write_(info);
	}
	else
	{
		res = write_(info_);
	}
	return res;
}

template<typename T, class Interface>
inline uint32 IOFrame<T, Interface>::frame_size_()const
{
	uint32 siz;
	if (scale_ > 1)
		siz = (info_.width / scale_)*(info_.height / scale_)*sizeof(T);
	else
		siz = info_.width * info_.height * sizeof(T);
	return siz;
}

template<typename T, class Interface>
inline bool IOFrame<T, Interface>::read_info(FrameInfo& info)
{
	bool res;
	if (read_(info))
	{
		if (scale_ > 1)
		{
			info.width *= scale_;
			info.height *= scale_;
		}
		info_ = info;
		res = true;
	}
	else
	{
		res = false;
	}
	return res;
}

template<typename T, class Interface>
template<typename U>
inline bool IOFrame<T, Interface>::read(Buffer<U>& buffer,
	header_type& header, bool force)
{
	bool res = false;
	if (read_header(header) || force)
	{
		int32 siz = frame_size_();
		int32 buffer_size = siz / sizeof(U);
		if (buffer.size() != buffer_size)
			buffer.resize(buffer_size);
		char* p_char = reinterpret_cast<char*>(buffer.data());
		int32  read_siz = Interface::read(p_char, siz);
		res = siz == read_siz;
		if (scale_ > 1)
			Sampler<U, T>::over(buffer, scale_, (info_.width * sizeof(T))/scale_);
	}
	return res;
}

template<typename T, class Interface>
template<typename U>
inline bool IOFrame<T, Interface>::write
	(const Buffer<U>& buffer,const header_type& header)
{
	bool res = false;
	if (write_header(header))
	{
		uint32 frame_siz = frame_size_();
		if (scale_ > 1)
		{
			Buffer<U> buffer_;
			Sampler<U, T>::sub(buffer, buffer_, scale_, info_.width * sizeof(T));
			res = Interface::write
				(reinterpret_cast<const char*>(buffer_.data()), frame_siz);
		}
		else
		{
			res = Interface::write
				(reinterpret_cast<const char*>(buffer.data()), frame_siz);
		}
	}
	return res;
}

/// \brief  Change the  of the next frame to read
template<typename T, class Interface>
int32 IOFrame<T, Interface>::get_index()
{
	bool change_pos = false;
	int32 info_siz = 0;

	int32 pos = Interface::get_pos();
	if (pos == 0)
		return 0;

	if (!IsKnown(info_.type))
	{
		change_pos = true;
		Interface::set_pos(0);
		read_info();
	}

	if (IsKnown(info_.type))
		info_siz = FrameInfo::static_size;

	uint32 frame_siz = frame_size_();
	int32 n_frames = 0;
	if (frame_siz > 0 && pos > info_siz)
	{
		frame_siz += FrameHeader::static_size;
		n_frames = (pos - info_siz) / frame_siz;
	}

	if (change_pos)
		Interface::set_pos(pos);

	return n_frames;
}

/// \brief  Change the  of the next frame to read
template<typename T, class Interface>
bool IOFrame<T, Interface>::set_index
(int32 count, bool relative)
{
	int32 info_siz = 0;
	int32 prev_pos = Interface::get_pos();

	if (!IsKnown(info_.type))
	{
		Interface::set_pos(0);
		if (read_info())
			info_siz = FrameInfo::static_size;
	}
	int32 frame_siz = frame_size_();
	int32 pos = count * frame_siz + info_siz;
	if (relative && prev_pos > info_siz)
	{
		prev_pos -= info_siz;
		pos += (prev_pos / frame_siz)*frame_siz;
	}
	return Interface::set_pos(pos);
}

/// \brief  Count the number of frames
template<typename T, class Interface>
int32 IOFrame<T, Interface>::count()
{
	bool was_open = false;
	bool change_pos = false;
	int32 pos = 0;
	int32 info_siz = 0;
	int32 siz = Interface::size();

	if (Interface::is_open())
		was_open = true;

	if (!IsKnown(info_.type))
	{
		change_pos = true;
		pos = Interface::get_pos();
		Interface::set_pos(0);
		read_info();
	}
	if (IsKnown(info_.type))
		info_siz = FrameInfo::static_size;

	int32 frame_siz = frame_size_();
	int32 n_frames = 0;
	if (frame_siz > 0)
	{
		frame_siz += FrameHeader::static_size;
		n_frames = (siz - info_siz) / frame_siz;
	}

	if (was_open)
	{
        if (change_pos)
            Interface::set_pos(pos);
	}
	else
		Interface::close();
	return n_frames;
}
} // sensor
#endif
