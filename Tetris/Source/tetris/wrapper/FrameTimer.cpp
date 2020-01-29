#include "tetris/wrapper/FrameTimer.h"

using namespace tetris::wrapper;



FrameTimer::FrameTimer() {
	this->currentFrames = 0;
	this->framesToTrigger = this->DEFAULT_TIMING_FRAMES;
}



FrameTimer::FrameTimer(int32_t frames) {
	this->currentFrames = 0;
	this->setTimingFrames(frames);
}



void FrameTimer::setTimingFrames(int32_t frames) {
	if (frames <= 0) {
		frames = this->DEFAULT_TIMING_FRAMES;
	}
	this->framesToTrigger = frames;
	this->currentFrames = this->currentFrames % this->framesToTrigger;
}



int32_t FrameTimer::getTimingFrames() const {
	return this->framesToTrigger;
}



void FrameTimer::update() {
	this->currentFrames = (this->currentFrames + 1) % this->framesToTrigger;
}



void FrameTimer::reset() {
	this->currentFrames = 0;
}



bool FrameTimer::isTriggered() const {
	return this->currentFrames == 0;
}

void FrameTimer::prepareToTrigger() {
	this->currentFrames = this->framesToTrigger - 1;
}



int32_t FrameTimer::getCurrentFrames() const {
	return this->currentFrames;
}
