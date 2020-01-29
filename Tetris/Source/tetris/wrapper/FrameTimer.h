#pragma once

#include <cstdint>

namespace tetris::wrapper {
	// Frame timer that will trigger once per specified amount of frames.
	// How to use:
	// 1) Create timer and set how often to trigger.
	// 2) Update the timer.
	// 3) Check if it is triggered.
	// 4) Repeat steps 2-4 as long as you want
	class FrameTimer {
	public:
		// How often (in frames) timer will work by default.
		static const int32_t DEFAULT_TIMING_FRAMES = 30;

		// Default constructor, works with default values.
		FrameTimer();
		// Sets how often (in frames) the timer is triggered.
		// Values less than or equal to zero set default values.
		FrameTimer(int32_t frames);

		// Sets how often (in frames) the timer is triggered.
		// Values less than or equal to zero set default values.
		void setTimingFrames(int32_t frames);

		// Returns how often this timer triggers.
		int32_t getTimingFrames()const ;

		// Updates the state of timer.
		void update();

		// Starts timer from beginning.
		void reset();

		// Returns true if timer is triggered in current iteration.
		// It may be triggered forever if not updated.
		bool isTriggered() const;

		// Makes the timer trigger after next update() call;
		void prepareToTrigger();

		// Returns current frames amount
		int32_t getCurrentFrames() const;
	private:
		int32_t currentFrames;
		int32_t framesToTrigger;
	};
}