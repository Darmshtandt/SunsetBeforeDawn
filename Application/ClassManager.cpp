#include <ClassManager.h>

ClassManager& ClassManager::Instance() noexcept {
	static ClassManager instance;
	return instance;
}

std::string ClassManager::GetName(const ClassID& id) const {
	if (m_Classes.contains(id))
		return m_Classes.at(id);
	return "Unknown";
}

Identifier::Identifier(const ClassID& id) noexcept :
	m_ID(id)
{
}

std::string Identifier::GetClassName() const {
	return ClassManager::Instance().GetName(m_ID);
}