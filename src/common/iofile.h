/// \file iofile.h
/// \brief Interface to read and write binary data in a File
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef SENSOR_IOFILE_H
#define SENSOR_IOFILE_H

#include "typedef.h"
#include <string>
#include <fstream>
#include <vector>

namespace sensor
{
typedef std::vector<char> CharBuffer;
/////////////////////////////////////////////////////////
/// \brief Base class for file access
class FileBase
{
protected:
	// Open mode
	std::fstream::openmode mode_;
	// Filename
	std::string filename_;
	// Stream
	mutable std::fstream ios_;
	/// \brief  Constructor
	explicit FileBase(const std::string& name) :
		mode_(std::fstream::binary),
		filename_(name)
	{}
	/// \brief Default constructor
	explicit FileBase() :
		mode_(std::fstream::binary)
		{}
	/// \brief Get current position
	int32 get_pos();
	/// \brief Change current position
	bool set_pos(int32 pos, 
		std::ios::seekdir = std::ios::beg);
public:
	/// \brief Destructor
	~FileBase();
	/// \brief Open the file
	bool open();
	/// Close the file
	void close();
	/// Test if the file is opened
	bool is_open()const;
	/// Get the file name
	const std::string& filename()const
	{
		return filename_;
	}
	/// \brief Change the filename
	void filename(const std::string& name);
	void filename(const std::wstring& wname)
	{
		filename(std::string(wname.begin(), wname.end()));
	}
	/// \brief File size
	int32 size()const;
	/// \brief  Test if end of file
	bool eof()const;
	/// \brief Set pos to the begining of the file
	bool set_begin()
	{
		return set_pos(0);
	}
	/// \brief Set pos to the end
	bool set_end()
	{
		return set_pos(0, std::ios::end);
	}
};
/////////////////////////////////////////////////////////
/// \brief Base class for reading
class ReadBase
{
protected:
	// Stream
	std::fstream& ios_;
	// Open mode
	std::fstream::openmode& mode_;
	// Filename
	std::string& filename_;
	/// \brief Constructor
	ReadBase(std::fstream& ios,
		std::string& filename,
		std::fstream::openmode& mode) :
		ios_(ios),mode_(mode),filename_(filename)
	{}
public:
	/// Read data
	int32 read(char* msg, int32 siz);
	int32 read(CharBuffer& buffer);
};
/// \brief Base class for writing
class WriteBase
{
protected:
	// Stream
	std::fstream& ios_;
	// Open mode
	std::fstream::openmode& mode_;
	// Filename
	std::string& filename_;
	/// \brief Constructor
	WriteBase(std::fstream& ios,
		std::string& filename,
		std::fstream::openmode& mode) :
		ios_(ios), mode_(mode), filename_(filename)
	{}
public:
	/// \brief  Write buffer
	bool write(const char* msg, int32 siz);
	bool write(const std::string& msg);
	bool write(const CharBuffer& buffer);
};
/////////////////////////////////////////////////////////
/// \brief Interface to read in a file
class IFile :
	public FileBase,
	public ReadBase
{
public:
	/// \brief Constructor
	explicit IFile(const std::string& name) :
		FileBase(name),
		ReadBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::in;
	}
	/// \brief Default constructor
	explicit IFile():
		ReadBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::in;
	}
	/// \brief Open with mode
	bool open(std::ofstream::openmode mode);
};
/////////////////////////////////////////////////////////
/// \brief Interface to read and write in a file
class OFile :
	public FileBase,
	public WriteBase
{
public:
	/// \brief Constructor
	explicit OFile(const std::string& name) :
		FileBase(name),
		WriteBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::out |
			std::fstream::trunc;
	}
	/// \brief Default constructor
	explicit OFile():
		WriteBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::out |
			std::fstream::trunc;
	}
	/// \brief Open with mode
	bool open(std::ofstream::openmode mode);
};
/////////////////////////////////////////////////////////
class IOFile :
	public FileBase,
	public ReadBase,
	public WriteBase
{
public:
	/// \brief Constructor
	explicit IOFile(const std::string& name) :
		FileBase(name),
		ReadBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_),
		WriteBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::out |
			std::fstream::in;
	}

	/// \brief Default constructor
	explicit IOFile():
		ReadBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_),
		WriteBase(FileBase::ios_,
			FileBase::filename_, FileBase::mode_)
	{
		FileBase::mode_ |= std::fstream::out |
			std::fstream::in;
	}
	/// \brief Open with mode
	bool open(std::ofstream::openmode mode);
};

// RawIOFileBase destructor
inline FileBase::~FileBase()
{
//    Lock lock(mutex_);
    if(ios_.is_open())
        ios_.close();
}
/// \brief Get current position
inline int32 FileBase::get_pos()
{
	if (ios_.is_open())
		return (int32)ios_.tellg();
	else
		return 0;
}

// Open the file
inline bool FileBase::open()
{
    ios_.open(filename_.c_str(),mode_);
	ios_.seekg(0,std::ios::beg);
    return ios_.is_open();
}
// Close the file
inline void FileBase::close()
{
    if (ios_.is_open())
        ios_.close();
}
// Test if the file is opened
inline bool FileBase::is_open()const
{
	return ios_.is_open();
}
// Test if end of file
inline bool FileBase::eof()const
{
	return ios_.eof();
}

inline int32 ReadBase::read(CharBuffer& buffer)
{
	return read(buffer.data(), buffer.size());
}
// Write data
inline bool WriteBase::write(const std::string& msg)
{
	return write(msg.c_str(), (int32)msg.size());
}
// Buffer data
inline bool WriteBase::write(const CharBuffer& buffer)
{
	return write(buffer.data(), (int32)buffer.size());
}

// Open with mode
inline bool OFile::open(std::ofstream::openmode mode)
{
	FileBase::mode_ = std::ofstream::out |
		std::ofstream::binary | mode;
	return FileBase::open();
}

// Open with mode
inline bool IOFile::open(std::fstream::openmode mode)
{
	FileBase::mode_ = std::ofstream::binary |
		mode;
	return FileBase::open();
}
}
#endif
