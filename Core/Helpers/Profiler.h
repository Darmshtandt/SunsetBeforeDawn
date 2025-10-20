#pragma once

#include <iostream>
#include <Nt/Core/Timer.h>

class Profiler {
public:
	Profiler() noexcept = default;

	[[nodiscard]]
	static Profiler& Instance() noexcept {
		static Profiler profiler;
		return profiler;
	}

	void Start() {
		m_Timer.Restart();
	}
	Double Stop(const std::string& text) {
		const Double seconds = Double(m_Timer.GetElapsedTimeMs()) / 1000.0;
		std::cout << text << ": " << seconds << " s" << std::endl;
		return seconds;
	}
	Double Stop() {
		return Double(m_Timer.GetElapsedTimeMs()) / 1000.0;
	}

private:
	Nt::Timer m_Timer;
};