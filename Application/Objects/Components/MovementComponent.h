#pragma once

#include <Nt/Core/Math/VectorND.h>
#include <Objects/Components/IComponent.h>

class MovementComponent final : public IComponent {
public:
	MovementComponent() noexcept;
	MovementComponent(const MovementComponent&) noexcept = default;
	MovementComponent(MovementComponent&&) noexcept = default;
	~MovementComponent() noexcept override = default;

	MovementComponent& operator = (const MovementComponent&) noexcept = default;
	MovementComponent& operator = (MovementComponent&&) noexcept = default;

public:
	Nt::Float3D DesiredVelocity;
};