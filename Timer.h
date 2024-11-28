#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>

class Timer {
public:
	Timer();
	~Timer();
	void Start(int interval, std::function<void()> callback);
	void Stop();

private:
	std::atomic<bool> running;
	std::thread thread;
};