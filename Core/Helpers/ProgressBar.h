#pragma once

#include <Nt/Core/Timer.h>
#include <mutex>
#include <unordered_map>

class ProgressBar final {
public:
	struct Task {
		[[nodiscard]] Double GetPercentage() const noexcept;

		Double Total = 0.0;
		std::atomic<Double> Progress = 0.0;
	};

public:
	ProgressBar(const ProgressBar&) = delete;
	ProgressBar(ProgressBar&&) = delete;

	ProgressBar& operator = (const ProgressBar&) = delete;
	ProgressBar& operator = (ProgressBar&&) = delete;

	[[nodiscard]] static ProgressBar& Instance() noexcept;

	void AddTask(const std::string& name, const Double& total);
	void Update(const std::string& name, const Double& amount);

protected:
	ProgressBar() noexcept = default;
	~ProgressBar() noexcept = default;

private:
	std::unordered_map<std::string, Task> m_TaskMap;
	std::unordered_map<std::string, uInt> m_IndexMap;
	std::mutex m_Mutex;
	Nt::Timer m_Timer;

private:
	void PrintProgress(const std::string& name);
};