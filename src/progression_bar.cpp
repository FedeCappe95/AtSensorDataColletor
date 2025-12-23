#include "progression_bar.hpp"
#include <stdint.h>
#include <cmath>
#include <iostream>
#include <format>
#include <array>


static const std::array TICK_SYMBOLS{
		'\\', '|', '/', '-', '|'
};
static size_t currentTickIndex = 0u;
static char currentTickSymbol = 0x00;
static size_t divisionFactor = 1u;


void progressionBarCallback(size_t done, size_t tot) {
	constexpr size_t width = 40;
	const size_t doneWidth = static_cast<size_t>(
		std::round(static_cast<float>(done) / static_cast<float>(tot) * static_cast<float>(width))
	);
	const size_t donePerc = static_cast<size_t>(
		std::round(static_cast<float>(done) / static_cast<float>(tot) * 100.0f)
	);
	const size_t notDoneWidth = width - doneWidth;
	std::cout << "[";
	for (size_t i = 0; i < doneWidth; ++i)
		std::cout << "#";
	for (size_t i = 0; i < notDoneWidth; ++i)
		std::cout << " ";
	std::cout << "] " << std::format("{:3}", donePerc) << "%";
	if (done >= tot)
		std::cout << '\n';
	else
		std::cout << '\r';
}

void resetTick(size_t divisionFactor_) {
	currentTickIndex = 0u;
	currentTickSymbol = 0x00;
	divisionFactor = divisionFactor_;
}

void tick() {
	const char newSymbol = TICK_SYMBOLS.at(currentTickIndex / divisionFactor);
	if (currentTickSymbol != newSymbol) {
		std::cout << newSymbol << '\r';
		currentTickSymbol = newSymbol;
	}
	++currentTickIndex;
	currentTickIndex = currentTickIndex % (TICK_SYMBOLS.size() * divisionFactor);
}
