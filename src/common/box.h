/// \file box.h
/// \brief Box class
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_BOX_H
#define CORE_BOX_H

#include "point.h"
#include "typedef.h"

namespace core
{

/// \brief Box
template<typename T = float_t,int32 I = 3>
struct Box
{
    enum
    {
        ndim = I
    };
    typedef T value_type;
	typedef Point<T,ndim> point_t;
    /// \brief Position min
    point_t position_min;
    /// \brief Position max
    point_t position_max;
    /// \brief Mean position
    point_t position()const
    {
        return (position_min + position_max)/T(2);
    }
    /// \brief Length
    point_t length()const
    {
        return position_max - position_min;
    }
    /// \brief Initialization
    void init()
    {
		position_max = point_t(Limit<T>::lowest());
        position_min = point_t(Limit<T>::greatest());
    }
    /// \brief Constructeurs
    Box()
    {
        init();
    }
    Box(const T* val,const T& l):
        position_min(val),
        position_max(val)
    {
        position_max += l;
    }
    Box(const T* val,const T* l):
        position_min(val),
        position_max(val)
    {
        position_max += point_t(l);
    }
};
/// \brief Test d'intersection de deux boîtes englobantes
template<typename T,int32 I >
bool Intersect(const Box<T,I>& box1, const Box<T,I>& box2)
{
    bool test = true;
    int32 i = 0;
    while(test && i < I)
    {
        test &=
            (box1.position_min[i] < box2.position_min[i] &&
             box1.position_max[i] > box2.position_min[i])||
            (box2.position_min[i] <= box1.position_min[i] &&
             box2.position_max[i] >= box1.position_min[i]);
        ++i;
    }
    return test;
}
/// \brief Box3D definition
template<typename T>
using Box3D = Box<T,3>;

/// \brief Box2D definition
template<typename T>
using  Box2D = Box<T,2>;
} // core
#endif

