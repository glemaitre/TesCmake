
// nodata.cpp
#include "nodata.h"

namespace core
{

template<>
const float32 CodeValue<float32>::no_data_ = CodeValueT<float32>::no_data();
template<>
const float32 CodeValue<float32>::infinite_ = CodeValueT<float32>::infinite();
template<>
const float32 CodeValue<float32>::unreliable_ = CodeValueT<float32>::unreliable();
template<>
const float32 CodeValue<float32>::masked_ = CodeValueT<float32>::masked();

template<>
const float64 CodeValue<float64>::no_data_ = CodeValueT<float64>::no_data();
template<>
const float64 CodeValue<float64>::infinite_ = CodeValueT<float64>::infinite();
template<>
const float64 CodeValue<float64>::unreliable_ = CodeValueT<float64>::unreliable();
template<>
const float64 CodeValue<float64>::masked_ = CodeValueT<float64>::masked();

template<>
const int8 CodeValue<int8>::no_data_ = CodeValueT<int8>::no_data();
template<>
const int8 CodeValue<int8>::infinite_ = CodeValueT<int8>::infinite();
template<>
const int8 CodeValue<int8>::unreliable_ = CodeValueT<int8>::unreliable();
template<>
const int8 CodeValue<int8>::masked_ = CodeValueT<int8>::masked();

template<>
const int16 CodeValue<int16>::no_data_ = CodeValueT<int16>::no_data();
template<>
const int16 CodeValue<int16>::infinite_ = CodeValueT<int16>::infinite();
template<>
const int16 CodeValue<int16>::unreliable_ = CodeValueT<int16>::unreliable();
template<>
const int16 CodeValue<int16>::masked_ = CodeValueT<int16>::masked();

template<>
const int32 CodeValue<int32>::no_data_ = CodeValueT<int32>::no_data();
template<>
const int32 CodeValue<int32>::infinite_ = CodeValueT<int32>::infinite();
template<>
const int32 CodeValue<int32>::unreliable_ = CodeValueT<int32>::unreliable();
template<>
const int32 CodeValue<int32>::masked_ = CodeValueT<int32>::masked();

template<>
const int64 CodeValue<int64>::no_data_ = CodeValueT<int64>::no_data();
template<>
const int64 CodeValue<int64>::infinite_ = CodeValueT<int64>::infinite();
template<>
const int64 CodeValue<int64>::unreliable_ = CodeValueT<int64>::unreliable();
template<>
const int64 CodeValue<int64>::masked_ = CodeValueT<int64>::masked();

template<>
const uint8 CodeValue<uint8>::no_data_ = CodeValueT<uint8>::no_data();
template<>
const uint8 CodeValue<uint8>::infinite_ = CodeValueT<uint8>::infinite();
template<>
const uint8 CodeValue<uint8>::unreliable_ = CodeValueT<uint8>::unreliable();
template<>
const uint8 CodeValue<uint8>::masked_ = CodeValueT<uint8>::masked();

template<>
const uint16 CodeValue<uint16>::no_data_ = CodeValueT<uint16>::no_data();
template<>
const uint16 CodeValue<uint16>::infinite_ = CodeValueT<uint16>::infinite();
template<>
const uint16 CodeValue<uint16>::unreliable_ = CodeValueT<uint16>::unreliable();
template<>
const uint16 CodeValue<uint16>::masked_ = CodeValueT<uint16>::masked();

template<>
const uint32 CodeValue<uint32>::no_data_ = CodeValueT<uint32>::no_data();
template<>
const uint32 CodeValue<uint32>::infinite_ = CodeValueT<uint32>::infinite();
template<>
const uint32 CodeValue<uint32>::unreliable_ = CodeValueT<uint32>::unreliable();
template<>
const uint32 CodeValue<uint32>::masked_ = CodeValueT<uint32>::masked();

template<>
const uint64 CodeValue<uint64>::no_data_ = CodeValueT<uint64>::no_data();
template<>
const uint64 CodeValue<uint64>::infinite_ = CodeValueT<uint64>::infinite();
template<>
const uint64 CodeValue<uint64>::unreliable_ = CodeValueT<uint64>::unreliable();
template<>
const uint64 CodeValue<uint64>::masked_ = CodeValueT<uint64>::masked();
}
