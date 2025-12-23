#ifndef __GICAME__NOT_COPYABLE_H__
#define __GICAME__NOT_COPYABLE_H__


#include "../common.h"


#define NOT_COPYABLE(T) \
    T(const T&) = delete; \
    T& operator=(const T&) = delete;


#endif