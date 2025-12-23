#ifndef __GICAME__DYNLIBRARY_H__
#define __GICAME__DYNLIBRARY_H__


#include "../common.h"
#include "../utils/NotCopyable.h"
#include <string>
#include <type_traits>
#include <new>
#include <filesystem>


namespace Gicame::Os {

	class DynLibraryHandler {

		NOT_COPYABLE(DynLibraryHandler)

	private:   // Private types
		using NativeHandle = void*;

	private:   // Private data memebers
		NativeHandle nativeHandle;

	private:   // Private methods
		constexpr DynLibraryHandler(NativeHandle nativeHandle_) : nativeHandle(nativeHandle_) {}
		[[noreturn]] void xLibraryNotLoaded();
		void unload();
		
	public:    // Public methods
		GICAME_API ~DynLibraryHandler();
		GICAME_API static DynLibraryHandler load(const std::filesystem::path& modulePath);
		GICAME_API void* getSymbolAddress(const char* symbolName);
		template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline T getSymbol(const char* symbolName) {
			return std::launder(reinterpret_cast<T>(getSymbolAddress(symbolName)));
		}

	};

};


#endif
