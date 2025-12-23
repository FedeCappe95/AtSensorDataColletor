#ifndef __GICAME__COMPARABLE_H__
#define __GICAME__COMPARABLE_H__


#include "Property.h"
#include <type_traits>
#include <utility>


namespace Gicame::Reflection {

	template<typename Derived>
	struct Comparable {

		inline bool equals(const Derived& o) const {
			// Check for properties
			static_assert(HasProperies<Derived>::has(), "Comparable requires properties");

			const Derived& self = *((Derived*)(this));

			// We first get the number of properties
			constexpr auto propertyCount = std::tuple_size<decltype(Derived::properties)>::value;
			constexpr auto propertyIndexes = std::make_index_sequence<propertyCount>{};

			bool equality = true;

			// We iterate on the index sequence of size propertyCount
			apply_on_index_sequence(propertyIndexes, [&](auto i) {
				constexpr auto prop = std::get<i>(Derived::properties);
				equality &= (self.*(prop.member) == o.*(prop.member));
			});

			return equality;
		}

		inline bool operator==(const Derived& o) const {
			return equals(o);
		}

		inline bool operator!=(const Derived& o) const {
			return !equals(o);
		}

	};

};


#endif
