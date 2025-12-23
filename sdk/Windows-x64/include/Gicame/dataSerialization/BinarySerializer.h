#ifndef __GICAME__BINARYSERIALIZER_H__
#define __GICAME__BINARYSERIALIZER_H__


#include "../common.h"
#include "../interfaces/ISerializable.h"
#include "../stream/ByteStream.h"
#include <vector>
#include <type_traits>


namespace Gicame {

	/**
	 * Generic binary serializer
	 */
	class GICAME_API BinarySerializer {

	public:
		constexpr BinarySerializer() {};

		template <class Type>
		std::vector<byte_t> serialize(const Type& data) const;    // Method A

		template <class Type>
		void serialize(const Type& data, void* outBuffer, const size_t outBufferSize) const;  // Method B

		template <class Type>
		void serialize(const Type& data, Stream::ByteOStream& out) const;  // Method C

		template <class Type>
		size_t serializedSize(const Type& data) const;

		template <class Type>
		size_t serializedSize() const;

		template <class Type>
		Type deserialize(const void* inBuffer, const size_t inBufferSize) const;

		template <class Type>
		Type deserialize(Stream::ByteIStream& in) const;

	};


	/*
	 * Inline implementation
	 */

	/**
	 * Serialization for ISerializable interface (Method A)
	 */
	template <>
	inline std::vector<byte_t> BinarySerializer::serialize(const ISerializable& data) const {
		return data.serialize();
	}

	/**
	 * Serialization for ISerializable interface (Method B)
	 */
	template <>
	inline void BinarySerializer::serialize(const ISerializable& data, void* outBuffer, const size_t outBufferSize) const {
		const std::vector<byte_t> dataBytes = data.serialize();
		const size_t dataSize = dataBytes.size();
		if (unlikely(dataSize > outBufferSize))
			throw RUNTIME_ERROR("outBuffer too short");
		for (size_t i = 0; i < dataSize; ++i)
			((byte_t*)outBuffer)[i] = dataBytes[i];
	}

	template <>
	inline void BinarySerializer::serialize(const ISerializable& data, Stream::ByteOStream& out) const {
		const std::vector<byte_t> dataBytes = data.serialize();
		out.write(dataBytes.data(), dataBytes.size());
	}

	template <>
	inline size_t BinarySerializer::serializedSize(const ISerializable& data) const {
		return data.serialize().size();  // Worst method possible, a better method is to be implemented
	}

	template <class Type>
	inline std::vector<byte_t> BinarySerializer::serialize(const Type& data) const {
		if constexpr (std::is_same<Type, void>::value) {
			return std::vector<byte_t>();;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			std::vector<byte_t> dataBytes(sizeof(Type));
			*((Type*)(dataBytes.data())) = data;
			return dataBytes;
		}
		else if constexpr (/*prop*/false) {
			return std::vector<byte_t>();  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

	template <class Type>
	inline void BinarySerializer::serialize(const Type& data, void* outBuffer, const size_t outBufferSize) const {
		if constexpr (std::is_same<Type, void>::value) {
			return;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			if (unlikely(outBufferSize < sizeof(Type)))
				throw RUNTIME_ERROR("outBuffer too short");
			*((Type*)(outBuffer)) = data;
		}
		else if constexpr (/*prop*/false) {
			return;  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

	template <class Type>
	inline void BinarySerializer::serialize(const Type& data, Stream::ByteOStream& out) const {
		if constexpr (std::is_same<Type, void>::value) {
			return;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			out.write((byte_t*)(&data), sizeof(Type));
		}
		else if constexpr (/*prop*/false) {
			return;  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

	template <class Type>
	inline size_t BinarySerializer::serializedSize(const Type&) const {
		return serializedSize<Type>();
	}

	template <class Type>
	inline size_t BinarySerializer::serializedSize() const {
		if constexpr (std::is_same<Type, void>::value) {
			return 0;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			return sizeof(Type);
		}
		else if constexpr (/*prop*/false) {
			return 0;  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

	template <class Type>
	inline Type BinarySerializer::deserialize(const void* inBuffer, const size_t inBufferSize) const {
		if constexpr (std::is_same<Type, void>::value) {
			return;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			if (unlikely(inBufferSize < sizeof(Type)))
				throw RUNTIME_ERROR("Invalid inBufferSize");
			const Type result = *((Type*)inBuffer);
			return result;
		}
		else if constexpr (/*prop*/false) {
			return Type{};  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

	template <class Type>
	inline Type BinarySerializer::deserialize(Stream::ByteIStream& in) const {
		if constexpr (std::is_same<Type, void>::value) {
			return;
		}
		else if constexpr (std::is_fundamental<Type>::value) {
			Type result;
			in.read((byte_t*)&result, sizeof(Type));
			if (unlikely(!in))
				throw RUNTIME_ERROR("Invalid inStream in");
			return result;
		}
		else if constexpr (/*prop*/false) {
			return Type{};  // TODO
		}
		else {
			throw RUNTIME_ERROR("Unsupported data type");
		}
	}

};


#endif
