#include <Engine/Input/InputBinder.h>
#include <Core/BusLocator.h>

InputMapper& InputMapper::Bind(const Int& key, const InputCommandID& command) {
	m_BindMap[key] = command;
	return *this;
}

std::unordered_set<InputCommandID> InputMapper::ToActions(const std::unordered_set<Int>& activeKeys) const {
	std::unordered_set<InputCommandID> commands;
	commands.reserve(activeKeys.size());

	for (const Int& key : activeKeys) {
		const auto it = m_BindMap.find(key);
		if (it != m_BindMap.cend())
			commands.insert(it->second);
	}

	return commands;
}

InputMapper& InputBinder::Context(const InputContextID& id) {
	return m_ContextMap[id];
}

void InputBinder::HandleKeys(const std::unordered_set<Int>& activeKeys) const {
	if (activeKeys.empty())
		return;

	for (const auto& [contextID, mapper] : m_ContextMap) {
		InputCommand input;
		input.ContextID = contextID;
		input.Commands = mapper.ToActions(activeKeys);

		if (!input.Commands.empty())
			BusLocator::ActionDispatcher().Emmit<InputCommand>(input);
	}
}
