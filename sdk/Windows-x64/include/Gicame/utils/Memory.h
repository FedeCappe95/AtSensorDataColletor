#ifndef __GICAME__MEMORY_H__
#define __GICAME__MEMORY_H__


#include "../common.h"
#include <stdint.h>
#include <new>
#include <memory>
#include <tuple>


// Classes and structs (utilities)
namespace Gicame::Utilities {

    static inline void eraseMemory(void* addr, size_t size) {
#ifdef __STDC_LIB_EXT1__

        memset_s(pointer, size, 0, size);

#else

        volatile byte_t* p = (volatile byte_t*)addr;
        while (size) {
            *p++ = 0;
            --size;
        }

#endif
    }

	template <typename T>
	static inline std::tuple<void*, size_t> align(void* ptr, size_t size) {
		ptr = std::align(alignof(T), sizeof(T), ptr, size);
		return { ptr, ptr ? size : 0u };
	}

	template <typename Ret = void>
	static inline const Ret* advance(const void* ptr, const size_t displacement) {
		return static_cast<const Ret*>(static_cast<const uint8_t*>(ptr) + displacement);
	}

	template <typename Ret = void>
	static inline Ret* advance(void* ptr, const size_t displacement) {
		return static_cast<Ret*>(static_cast<uint8_t*>(ptr) + displacement);
	}

}


#endif