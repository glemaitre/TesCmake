/// \file abs.h
/// \brief Template version of the abs function
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef UTIL_ABS_H
#define UTIL_ABS_H

namespace util
{
    template<typename T>
    T Abs(const T& val)
    {
        return (val > 0)? val : -val;
    }
} // util
#endif
