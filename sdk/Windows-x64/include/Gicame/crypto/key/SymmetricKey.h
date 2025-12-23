#ifndef __GICAME__SYMMETRICKEY_H__
#define __GICAME__SYMMETRICKEY_H__


#include <memory>
#include <vector>
#include "../../common.h"
#include "../common/IAlgorithmDescriptor.h"


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API SymmetricKey {

	private:
#ifdef MSVC
#pragma warning(push)
#pragma warning(disable:4251)
#endif
		std::shared_ptr<const byte_t> buffPtr;
#ifdef MSVC
#pragma warning(pop)
#endif
		const size_t keySize;

	private:
		SymmetricKey(const std::shared_ptr<const byte_t>& buffPtr, const size_t keySize);

	public:
		const byte_t* get() const;
		size_t getKeySize() const;

	public:
		static SymmetricKey createSymmetricKey(const byte_t* keyBuffer, const size_t keyBufferSize, const IAlgorithmDescriptor& algorithm);
		static SymmetricKey createSymmetricKeyTakeOwnership(const byte_t* keyBuffer, const size_t keyBufferSize, const IAlgorithmDescriptor& algorithm);

	};

};


#endif
