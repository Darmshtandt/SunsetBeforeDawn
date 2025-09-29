#pragma once

template <class _Ty>
std::string ClassManager::GetName() const {
	if (m_Classes.contains(Class<_Ty>::ID()))
		return m_Classes.at(Class<_Ty>::ID());
	return "Unknown";
}

template <class _Ty>
void ClassManager::BindName(std::string name) {
	m_Classes[Class<_Ty>::ID()] = std::move(name);
}

template <class _Ty>
constexpr ClassID Class<_Ty>::ID() noexcept {
	std::size_t hash = 1469598103934665603ull;
	for (const Char& symbol : __FUNCSIG__) {
		hash ^= static_cast<uInt>(symbol);
		hash *= 1099511628211ull;
	}
	return hash;
}

template <class _Ty>
template <class _U>
constexpr Bool Class<_Ty>::Is() noexcept {
	return ID() == Class<_U>::ID();
}

template <class _Ty>
constexpr Bool Class<_Ty>::Is(const ClassID& otherID) noexcept {
	return ID() == otherID;
}

template <class _Ty>
constexpr Bool Class<_Ty>::Is(const Identifier& other) noexcept {
	return ID() == other.GetID();
}