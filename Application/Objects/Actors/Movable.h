#pragma once

#include <Nt/Core/Math/Vector.h>

class Movable {
public:
	Movable() noexcept = default;
	Movable(const Movable&) noexcept = default;
	Movable(Movable&&) noexcept = default;
	virtual ~Movable() noexcept = default;

	Movable& operator = (const Movable&) noexcept = default;
	Movable& operator = (Movable&&) noexcept = default;

	virtual void StartMoving();

	virtual void Move(const Nt::Float3D& velocity) = 0;
	virtual void ApplyRotation(const Nt::Float3D& angle) = 0;

	virtual void StopMoving();

	[[nodiscard]] virtual Float GetSpeed() const noexcept = 0;
	[[nodiscard]] virtual Float GetPitch() const noexcept = 0;
};