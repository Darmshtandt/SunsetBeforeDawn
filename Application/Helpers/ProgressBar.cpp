#include <Helpers/ProgressBar.h>
#include <iostream>
#include <iomanip>

Double ProgressBar::Task::GetPercentage() const noexcept {
	return Progress / Total;
}

ProgressBar& ProgressBar::Instance() noexcept {
	static ProgressBar progressBar;
	return progressBar;
}

void ProgressBar::AddTask(const std::string& name, const Double& total) {
	std::lock_guard lock(m_Mutex);
	if (!m_TaskMap.contains(name))
		std::cout << "\n";

	m_IndexMap[name] = m_TaskMap.size();
	m_TaskMap[name].Total = total;
}

void ProgressBar::Update(const std::string& name, const Double& amount) {
	std::lock_guard lock(m_Mutex);
	if (!m_TaskMap.contains(name))
		return;

	m_TaskMap[name].Progress += amount;

	const Bool isComplited =
		m_TaskMap[name].Progress == m_TaskMap[name].Total;
	if (m_Timer.GetElapsedTimeMs() > 500 || isComplited) {
		const Int linesUp = m_TaskMap.size() - m_IndexMap[name];
		std::cout << "\033[" << linesUp << "F";

		PrintProgress(name);

		std::cout << "\033[" << linesUp << "E";
		std::cout.flush();

		m_Timer.Restart();
	}
}

void ProgressBar::PrintProgress(const std::string& name) {
	const Task& task = m_TaskMap[name];

	constexpr Double barWidth = 30.0;
	const Double progress = task.Progress / task.Total;
	const uInt posiotion = uInt(barWidth * progress);

	std::cout << "\033[K";
	std::cout << name << " "
		<< std::setw(3) << Int(progress * 100) << "%|";

	for (uInt i = 0; i < posiotion; ++i)
		std::cout << "█";

	for (uInt i = posiotion; i < barWidth; ++i)
		std::cout << ' ';

	std::cout << "| ";

	std::cout << std::fixed << std::setprecision(1)
		<< task.Progress << "/" << task.Total;
}
