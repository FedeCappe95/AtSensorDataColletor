#ifndef __GICAME__SERIALIZATION__TREE_H__
#define __GICAME__SERIALIZATION__TREE_H__


#include "../common.h"
#include <vector>
#include <variant>
#include <string>
#include <any>
#include <type_traits>
#include "../reflection/Property.h"


namespace Gicame::Serialization::Tree {

	struct Leaf {
		std::string_view name;
		std::any value;

		template<typename Type>
		constexpr Leaf(std::string_view&& name, Type&& o) : name{ name }, value { o } {}
		template<typename Type>
		constexpr Leaf(const std::string_view& name, Type&& o) : name{ name }, value{ o } {}
	};

	struct Node {
		std::string_view name;
		std::vector<std::variant<Leaf, Node>> children;

		inline Node(const std::string_view& name) : name{ name } {}
		inline Node(std::string_view&& name) : name{ name } {}
	};

	struct Tree {
		Node root;
	};

	template<typename OType>
	static inline Node buildTreeNode(const std::string_view& name, const OType& object) {
		// Check for properties
		static_assert(Gicame::Reflection::HasProperies<OType>::has(), "Trying to build a tree from an object without properties");

		// JSON root
		Node node(name);

		// We first get the number of properties
		constexpr auto propertyCount = std::tuple_size<decltype(OType::properties)>::value;
		constexpr auto propertyIndexes = std::make_index_sequence<propertyCount>{};

		// We iterate on the index sequence of size propertyCount
		Gicame::Reflection::apply_on_index_sequence(propertyIndexes, [&](auto i) {
			constexpr auto prop = std::get<i>(OType::properties);
			using PropType = decltype(prop);
			using PropMemberType = PropType::template MemberType;

			const std::string_view childNodeName(prop.name);

			if constexpr (Gicame::Reflection::HasProperies<PropMemberType>::has()) {
				node.children.emplace_back(buildTreeNode(childNodeName, object.*(prop.member)));
			}
			else {
				Leaf leaf(childNodeName, object.*(prop.member));
				node.children.emplace_back(std::move(leaf));
			}
		});

		return node;
	}

	template<typename Type>
	static inline Tree buildTree(const std::string_view& name, const Type& o) {
		return Tree{ buildTreeNode(name, o) };
	}

};


#endif
