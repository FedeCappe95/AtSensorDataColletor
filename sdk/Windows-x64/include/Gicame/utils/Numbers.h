#ifndef __GICAME__NUMBERS_H__
#define __GICAME__NUMBERS_H__


#include <stdint.h>
#include <limits>
#include <stdexcept>
#include <type_traits>


#define NUMBERS_RUNTIME_ERROR(MSG) std::runtime_error(std::string(__FUNCTION__) + "(...): " + (MSG))


// Classes and structs (utilities)
namespace Gicame::Utilities {

    template<typename Type, typename OutType>
    static constexpr OutType maxOf() {
        return static_cast<OutType>(std::numeric_limits<Type>::max());
    }

    template<typename Type, typename OutType>
    static constexpr OutType minOf() {
        return static_cast<OutType>(std::numeric_limits<Type>::min());
    }

#ifdef MSVC
#pragma warning(push)
#pragma warning (disable : 4702)
#endif

    template<typename Target, typename Original>
    static inline Target safeNumericCast(const Original original) {
        if constexpr (std::is_same<Target, Original>::value)
            return original;

        // Both unsigned or both signed, always safe
        if constexpr (std::is_signed_v<Target> == std::is_signed_v<Original> && sizeof(Target) >= sizeof(Original)) {
            return (Target)original;
        }

        // Both unsigned
        if constexpr (!std::is_signed_v<Target> && !std::is_signed_v<Original>) {
            if (original > std::numeric_limits<Target>::max())
                throw NUMBERS_RUNTIME_ERROR("original too big");
        }

        // Both signed
        if constexpr (std::is_signed_v<Target> && std::is_signed_v<Original>) {
            if (int64_t(original) > maxOf<Target, int64_t>())
                throw NUMBERS_RUNTIME_ERROR("original too positive");
            if (int64_t(original) < minOf<Target, int64_t>())
                throw NUMBERS_RUNTIME_ERROR("original too negative");
        }

        // From signed to unsigned
        if constexpr (!std::is_signed_v<Target> && std::is_signed_v<Original>) {
            if (original < 0)
                throw NUMBERS_RUNTIME_ERROR("original is negative and Target is unsigned");
            if (uint64_t(original) > maxOf<Target, uint64_t>())
                throw NUMBERS_RUNTIME_ERROR("original too big");
        }

        // From unsigned to signed
        if constexpr (std::is_signed_v<Target> && !std::is_signed_v<Original>)
            if (uint64_t(original) > maxOf<Target, uint64_t>())
                throw NUMBERS_RUNTIME_ERROR("original too big");

        return (Target)original;
    }

#ifdef MSVC
#pragma warning (pop)
#endif

}


#undef NUMBERS_RUNTIME_ERROR


#endif