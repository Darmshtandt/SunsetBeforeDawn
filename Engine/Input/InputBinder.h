#pragma once

#include <Core/Input/Commands.h>
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

	InputMapper& Bind(const Int& key, const InputCommandID& command);

	[[nodiscard]]
	std::unordered_set<InputCommandID> ToActions(const std::unordered_set<Int>& activeKeys) const;

private:
	std::unordered_map<Int, InputCommandID> m_BindMap;
};

class InputBinder final {
public:
	InputBinder() noexcept = default;
	InputBinder(const InputBinder&) noexcept = default;
	InputBinder(InputBinder&&) noexcept = default;
	~InputBinder() noexcept = default;

	InputBinder& operator = (const InputBinder&) noexcept = default;
	InputBinder& operator = (InputBinder&&) noexcept = default;

	InputMapper& Context(const InputContextID& id);

	void HandleKeys(const std::unordered_set<Int>& activeKeys) const;

private:
	std::unordered_map<InputContextID, InputMapper> m_ContextMap;
};