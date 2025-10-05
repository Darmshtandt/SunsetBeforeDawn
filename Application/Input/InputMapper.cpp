#include <Input/InputMapper.h>

void InputMapper::Bind(const Int& key, const Command& command) {
	m_BindMap[key] = command;
}

std::unordered_set<Command> InputMapper::ToActions(const std::unordered_set<Int>& activeKeys) const {
	std::unordered_set<Command> commands;
	commands.reserve(activeKeys.size());

	for (const Int& key : activeKeys) {
		const auto it = m_BindMap.find(key);
		if (it != m_BindMap.cend())
			commands.insert(it->second);
	}

	return commands;
}