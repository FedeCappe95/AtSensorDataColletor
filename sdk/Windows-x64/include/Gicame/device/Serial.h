#ifndef __GICAME__SERIAL_H__
#define __GICAME__SERIAL_H__


#include <stdint.h>
#include <stdexcept>
#include "../common.h"
#include "../utils/NotCopyable.h"
#include "../interfaces/IDataExchanger.h"


namespace Gicame::Device {

	class GICAME_API Serial : public Gicame::IDataExchanger {

		NOT_COPYABLE(Serial);

	public:
		static constexpr uint32_t P_NOPARITY = 0;
		static constexpr uint32_t P_ODDPARITY = 1;
		static constexpr uint32_t P_EVENPARITY = 2;
		static constexpr uint32_t P_MARKPARITY = 3;
		static constexpr uint32_t P_SPACEPARITY = 4;
		static constexpr uint32_t SB_ONESTOPBIT = 0;
		static constexpr uint32_t SB_ONE5STOPBITS = 1;
		static constexpr uint32_t SB_TWOSTOPBITS = 2;

	public:
		struct SerialPort {
			std::string name;
			uint32_t index;
			inline SerialPort(const std::string& name, const uint32_t index) : name(name), index(index) {}
		};

	private:
		void* handle;
		const uint32_t comIndex;
		const uint32_t boud;
		const uint32_t byteSize;
		const uint32_t parity;
		const uint32_t stopBitsMode;
		bool blockingRecpt;

	public:
		Serial(const uint32_t comIndex, const uint32_t boud, const uint32_t byteSize = 8, const uint32_t parity = 0, const uint32_t stopBitsMode = 0);

		/**
		 * Open a serial communication
		 * @return true if the communication can be established
		 */
		bool open();

		virtual bool isReceiverConnected() const override final;
		virtual bool isSenderConnected() const override final;
		virtual size_t send(const void* buffer, const size_t size) override final;
		virtual size_t receive(void* buffer, const size_t size) override final;
		void clear(const bool rx, const bool tx);
		void flush();

		/**
		 * Close the serial communication is previously open
		 */
		void close();

		/**
		 * Set the receptionBlocking flag: when receiving, try to receive the exact amount of bytes
		 * specified
		 */
		void setReceptionBlocking(const bool rb);

	public:
		static std::vector<SerialPort> enumerateSerialPorts();

	};

};


#endif
