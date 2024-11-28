#include "Timer.h"

Timer::Timer() : running(false) {}

Timer::~Timer() { Stop(); }

void Timer::Start(int interval, std::function<void()> callback) {
	if (running) { return; }
	running = true;
	thread = std::thread([this, interval, callback]() {
		while (running) {
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			callback();
		}
	});
}

void Timer::Stop() {
	if (running) { running = false; }
	if (thread.joinable()) { thread.join(); }
}