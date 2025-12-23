#ifndef __GICAME__PROPERTY_H__
#define __GICAME__PROPERTY_H__


#include <type_traits>


namespace Gicame::Reflection {

	template<typename Class, typename T>
	struct Property {

		using MemberType = T;
		T Class::*member;
		const char* name;

		constexpr Property(T Class::*member, const char* name) : member{ member }, name{ name } {}

		static T getPropValueType();

	};

	template<typename Class, typename T>
	constexpr auto makeProperty(T Class::*member, const char* name) {
		return Property<Class, T>{member, name};
	}


	/*
	 * @brief Detector for static data member "properties"
	 */
	template <class T>
	class HasProperies {

	private:
		template<class U, class = typename std::enable_if<!std::is_member_pointer<decltype(&U::properties)>::value>::type>
		static std::true_type check(int);   // definition omitted

		template <class>
		static std::false_type check(...);  // definition omitted

	public:
		static constexpr bool value = decltype(check<T>(0))::value;
		static constexpr bool has() { return value; }

	};


	template <typename T, T... S, typename F>
	constexpr void apply_on_index_sequence(std::integer_sequence<T, S...>, F&& f) {
		using unpack_t = int[];
		(void)unpack_t {
			(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
		};
	}

};


#endif
