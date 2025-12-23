#include "Gicame/device/Serial.h"
#include "progression_bar.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <optional>
#include <ranges>
#include <algorithm>
#include <format>
#include <array>


using namespace Gicame::Device;


#define STRLIT_TO_COUPLE(S) (S), sizeof(S)


static std::optional<Serial::SerialPort> getNewPort(
    const std::vector<Serial::SerialPort>& portsBefore,
    const std::vector<Serial::SerialPort>& portsAfter
) {
    std::optional<Serial::SerialPort> winner;

    std::vector<Serial::SerialPort> newPorts;
    for (const Serial::SerialPort& serialPort : portsAfter) {
        auto matchPredicate = [&](const Serial::SerialPort& p) { return p.index == serialPort.index; };
        if (std::find_if(portsBefore.begin(), portsBefore.end(), matchPredicate) == portsBefore.end()) {
            newPorts.emplace_back(serialPort);
        }
    }

    if (newPorts.size() == 1u)
        winner.emplace(newPorts[0]);

    return winner;
}


//static void clear(Serial& serial) {
//    serial.setReceptionBlocking(false);
//    char buff[512];
//    size_t receivedBytes;
//    do {
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        receivedBytes = serial.receive(buff, sizeof(buff));
//    } while (receivedBytes != 0u);
//    serial.setReceptionBlocking(true);
//}

static std::string receiveStringUpTo(Serial& serial, const std::string_view& marker) {
    char buffer[512];
    memset(&buffer[0], 0, sizeof(buffer));

    char* ptr = &buffer[0];
    for (;;) {
        if (ptr == &buffer[sizeof(buffer) - 2u])
            break;

        const size_t r = serial.receive(ptr, 1);
        if (r == 0)
            continue;

        ptr += r;

        const size_t totRead = ptr - &buffer[0];
        if (totRead >= marker.size()) {
            const std::string_view lastPart(&buffer[totRead - marker.size()]);
            if (lastPart == marker)
                break;
        }
    }

    return std::string(buffer);
}

static std::string sendCommandAndReadLine(Serial& serial, const std::string_view& cmd, const std::string_view& replyPrefix) {
    // Send command
    serial.send(cmd.data(), cmd.size());

    // Read and validate echo
    const std::string echo = receiveStringUpTo(serial, "\n");
    if (echo != cmd) {
        std::cerr << "Invalid reply" << std::endl;
        exit(1);
    }

    // Read reply
    std::string line = receiveStringUpTo(serial, "\n");
    if (line == "\n")
        line = receiveStringUpTo(serial, "\n");

    // Discard remaining device output
    receiveStringUpTo(serial, "OK\n");

    // Parse reply
    if (line.ends_with("\n"))
        line = line.substr(0, line.length() - 1u);
    if (line.starts_with(replyPrefix))
        line = line.substr(replyPrefix.size());
    else
        line = std::string("Invalid reply (") + line + ")";

    return line;
}

static void waitUser(const std::string_view& msg) {
    std::cout << msg << std::endl;
    std::string line;
    std::getline(std::cin, line);
}

int main() {
    std::cout << "==================================\n";
    std::cout << "=== AtSensorDataCollector v1.0 ===\n";
    std::cout << "==================================\n\n";

    auto portsBefore = Serial::enumerateSerialPorts();

    std::cout << "Please, connect the usb device\n";

    // Select COM port
    resetTick(10u);
    uint32_t comPortIndex;
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        tick();
        auto portsAfter = Serial::enumerateSerialPorts();
        const auto winner = getNewPort(portsBefore, portsAfter);
        if (winner.has_value()) {
            std::cout << std::format("\nConnecting to {} (COM{})...", winner.value().name, winner.value().index);
            std::cout << std::endl;
            comPortIndex = winner.value().index;
            break;
        }
        portsBefore = portsAfter;
    }

    // Wait device initialization
    std::cout << "Waiting device initialization (1/2)...\n";
    for (size_t i = 0; i < 100u; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        progressionBarCallback(i + 1u, 100u);
    }

    // Open serial communication
    Serial serial(comPortIndex, 115200u, 8u, Serial::P_NOPARITY, Serial::SB_ONESTOPBIT);
    serial.open();
    serial.send(STRLIT_TO_COUPLE("ATE\n"));
    std::cout << "Waiting device initialization (2/2)...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    serial.clear(true, false);

    // Read info
    serial.setReceptionBlocking(true);
    std::cout << "  - Firmware version: " << sendCommandAndReadLine(serial, "AT+VER=?\n", "AT+VER=") << std::endl;
    std::cout << "  - Join EUI: " << sendCommandAndReadLine(serial, "AT+APPEUI=?\n", "AT+APPEUI=") << std::endl;
    std::cout << "  - Device EUI: " << sendCommandAndReadLine(serial, "AT+DEVEUI=?\n", "AT+DEVEUI=") << std::endl;
    std::cout << "  - AppKey: " << sendCommandAndReadLine(serial, "AT+APPKEY=?\n", "AT+APPKEY=") << std::endl;

    waitUser("Press enter to close the program");

    return 0;
}
