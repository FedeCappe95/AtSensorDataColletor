#ifndef __GICAME__SHAREDMEMORY_H__
#define __GICAME__SHAREDMEMORY_H__


#include <stdint.h>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <new>
#include "../common.h"
#include "../utils/Memory.h"


namespace Gicame {

	class SharedMemory {

	private:
		const std::string name;
		const size_t size;
		void* ptr;
#ifdef WINDOWS
		void* hFile;
		void* hMap;
#else
		int fd;
#endif
		bool unlinkOnDestruction;

	public:
		GICAME_API SharedMemory(const std::string& name, const size_t size);
		GICAME_API ~SharedMemory();
		GICAME_API void close();
		GICAME_API bool open(const bool createIfNotExisting);
		GICAME_API void unlink();
		GICAME_API void setUnlinkOnDestruction(bool uod);
		inline void* get() { return ptr; }
		inline size_t getSize() { return size; }

		/**
		 * @brief Return the memory area as a pointer to type T
		 *
		 * @param construct If true, the resulting pointer points to an object
		 *                  of type T whose life cicle begins with a new, so
		 *                  it is fully initialized. If false, the user
		 *                  assumes the memory area contains an already
		 *                  initialized object. Please, be extremely cautious
		 *                  about setting this parameter to false.
		 * @return A pointer to an object of type T in case of success, NULL
		 *         otherwise.
		 */
		template<class T, typename = std::enable_if<std::is_trivial_v<T> && std::is_standard_layout_v<T>>>
		inline T* getAs(const bool construct) {
			auto [objPtr, resultingSize] = Gicame::Utilities::align<T>(ptr, size);
			if (!objPtr)
				return NULL;
			if (construct)
				return new (objPtr) T;
			return std::launder(reinterpret_cast<T*>(objPtr));
		}

	};

};


#endif
