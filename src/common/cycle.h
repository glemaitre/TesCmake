////////////////////////////////////////////////////////
/// \file cycle.h
/// \brief cpu cycle count
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////
#ifndef CYCLE_H
#define CYCLE_H

#include "typedef.h"
#include <stdint.h>

//  Windows
#ifdef _WIN32

#include <intrin.h>
inline uint64 getticks(){
    return __rdtsc();
}

//  Linux/GCC
#else

inline uint64 getticks(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64)hi << 32) | lo;
}

#endif

inline uint64 elapsed(uint64 t1,uint64 t0)
{
    return t1-t0;
}
#endif
