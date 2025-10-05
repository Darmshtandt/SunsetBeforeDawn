#pragma once

#include <Input/Commands.h>
#include <unordered_map>
#include <unordered_set>

class InputMapper final {
public:
	InputMapper() noexcept = default;
	InputMapper(const InputMapper&) noexcept = default;
	InputMapper(InputMapper&&) noexcept = default;
	~InputMapper() noexcept = default;

	InputMapper& operator = (const InputMapper&) noexcept = default;
	InputMapper& operator = (InputMapper&&) noexcept = default;

	void Bind(const Int& key, const Command& command);

	[[nodiscard]]
	std::unordered_set<Command> ToActions(const std::unordered_set<Int>& activeKeys) const;

private:
	std::unordered_map<Int, Command> m_BindMap;
};
