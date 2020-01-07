#pragma once

#include <cstdint>

namespace tetris {
	enum class FigureType : int32_t {
		// No figure.
		None = 0,

		// Bright figure (for J and S pieces).
		Bright = 1,

		// Dark figure (for L and Z pieces).
		Dark = 2,

		// Neutral white figure (for O, T and I pieces).
		White = 3,
	};
}