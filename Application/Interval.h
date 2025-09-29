#pragma once

#include <Nt/Core/Timer.h>
#include <functional>

class Interval {
public:
	using Callback = std::function<void()>;

public:
	Interval(const uInt& minMs, const uInt& maxMs, Callback callback);

	Interval() = delete;
	Interval(const Interval&) = default;
	Interval(Interval&&) noexcept = default;
	~Interval() noexcept = default;

	Interval& operator = (const Interval&) noexcept = default;
	Interval& operator = (Interval&&) noexcept = default;

	void Stop() noexcept;
	void Start() noexcept;

	void Update();

	void SetCallback(Callback onIntervalElapsed);

private:
	Callback m_OnIntervalElapsed;
	Nt::Timer m_Timer;
	uInt m_DelayMs;
	uInt m_MinMs;
	uInt m_MaxMs;
	Bool m_Active = true;

private:
	[[nodiscard]] uInt _GetRandom() const noexcept;
};
