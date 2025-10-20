#pragma once

#include <Objects/GameObject.h>

class DebugSpectator final : public GameObject {
public:
	DebugSpectator() noexcept :
		GameObject(Class<DebugSpectator>::ID(), ObjectType::Debug)
	{
	}

	DebugSpectator(const DebugSpectator&) = delete;
	DebugSpectator(DebugSpectator&&) noexcept = default;
	~DebugSpectator() noexcept override = default;

	DebugSpectator& operator = (const DebugSpectator&) = delete;
	DebugSpectator& operator = (DebugSpectator&&) noexcept = default;

	void Update(const Float& deltaTime) override {
		(void)deltaTime;
	}

	void Increase() noexcept {
		m_Speed *= 2.f;
	}
	void Decrease() noexcept {
		m_Speed /= 2.f;
	}

	[[nodiscard]] Float GetSpeed() const noexcept {
		return m_Speed;
	}

private:
	Float m_Speed = 5.25f;
};