#pragma once

#include <Objects/Actors/Movable.h>
#include <Objects/GameObject.h>

class DebugSpectator final : public GameObject, public Movable {
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

	void Move(const Nt::Float3D& velocity) noexcept override {
		Translate(velocity);
	}
	void ApplyRotation(const Nt::Float3D& angle) override {
		Rotate(angle);
	}

	void Increase() noexcept {
		m_Speed *= 2.f;
	}
	void Decrease() noexcept {
		m_Speed /= 2.f;
	}

	[[nodiscard]] Float GetSpeed() const noexcept override {
		return m_Speed;
	}

	Float GetPitch() const noexcept override {
		return m_Angle.y;
	}

private:
	Float m_Speed = 5.25f;
};