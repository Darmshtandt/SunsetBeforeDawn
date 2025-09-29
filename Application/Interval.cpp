#include <Interval.h>

Interval::Interval(const uInt& minMs, const uInt& maxMs, Callback callback) :
	m_OnIntervalElapsed(std::move(callback)),
	m_MinMs(minMs),
	m_MaxMs(maxMs)
{
	Assert(m_OnIntervalElapsed, "Invalid Callback");

	m_Timer.Restart();
	m_DelayMs = _GetRandom();
}

void Interval::Stop() noexcept {
	if (m_Active)
		m_Active = false;
}

void Interval::Start() noexcept {
	if (!m_Active) {
		m_Timer.Restart();
		m_Active = true;
	}
}

void Interval::Update() {
	if (m_Active && m_Timer.GetElapsedTimeMs() >= m_DelayMs) {
		m_OnIntervalElapsed();
		m_DelayMs = _GetRandom();
		m_Timer.Restart();
	}
}

void Interval::SetCallback(Callback onIntervalElapsed) {
	Assert(onIntervalElapsed, "Invalid Callback");
	m_OnIntervalElapsed = std::move(onIntervalElapsed);
}

uInt Interval::_GetRandom() const noexcept {
	if (m_MinMs == m_MaxMs)
		return m_MinMs;
	return m_MinMs + (static_cast<uInt>(rand()) % (m_MaxMs - m_MinMs + 1));
}
