#include <Input/InputMapper.h>

void InputMapper::Bind(const Int& key, const Command& command) {
	m_BindMap[key] = command;
}
void InputMapper::Bind(const Nt::Key& key, const Command& command) {
	m_BindMap[static_cast<Int>(key)] = command;
}

std::unordered_set<Command> InputMapper::ToActions(const std::unordered_set<Int>& activeKeys) const {
	std::unordered_set<Command> commands;
	commands.reserve(activeKeys.size());

	for (const Int& key : activeKeys)
		commands.insert(m_BindMap.at(key));

	return commands;
}