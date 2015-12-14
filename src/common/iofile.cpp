
// iofile.cpp
#include "iofile.h"

namespace sensor
{
// Change the filename
	void FileBase::filename(const std::string& name)
{
	if (filename_ == name)
		return;
	if (ios_.is_open())
		ios_.close();
	filename_ = name;
}

/// \brief Set to the beginning of the file
bool FileBase::set_pos(int32 pos,std::ios::seekdir dir)
{
	bool check;
	if (!ios_.is_open())
	{
		ios_.open(filename_.c_str(),mode_);
	}
	if (ios_.is_open())
	{
		ios_.seekg(pos,dir);
		check = true;
	}
	else
	{
		check = false;
	}
	return check;
}

// File size
int32 FileBase::size()const
{
	int32 len;
	if (ios_.is_open())
	{
		int32 pos;
		pos = (int32)ios_.tellg();
		ios_.seekg(0, std::ios::end);
		len = (int32)ios_.tellg();
		ios_.seekg(pos, std::ios::beg);
	}
	else
	{
		ios_.open(filename_.c_str(),mode_);
		if (ios_.is_open())
		{
			ios_.seekg(0, std::ios::end);
			len = (int32)ios_.tellg();
			ios_.close();
		}
		else
		{
			len = 0;
		}
	}
	return len;
}
// Read data
int32 ReadBase::read(char* msg, int32 siz)
{
    if(!ios_.is_open())
	{
		mode_ ^= std::fstream::out;
		mode_ |= std::fstream::in;
        ios_.open(filename_.c_str(),mode_);
		ios_.seekg(0,std::ios::beg);
	}
    if(ios_.is_open())
    {
        int32 pos,len;
        pos = (int32) ios_.tellg();
        ios_.seekg (0, std::ios::end);
		len = (int32) ios_.tellg();
        ios_.seekg (pos, std::ios::beg);

        if(len < siz)
            siz = len;
		ios_.read(msg, siz);

        if(!ios_.good())
            siz = 0;
    }
    else
    {
        siz = 0;
    }
    return siz;
}

// Write data
bool WriteBase::write(const char* msg, int32 siz)
{
    bool test;
    // Tentative d'ouverture
    if(!ios_.is_open())
    {
		mode_ ^= std::fstream::in;
		mode_ |= std::fstream::out;
        ios_.open(filename_.c_str(),mode_);
    }
    if(ios_.is_open())
    {
        test = true;
        // Write the message
        ios_.write(msg,siz);
    }
    else
    {
        test = false;
    }
    return test;
}
} // sensor
