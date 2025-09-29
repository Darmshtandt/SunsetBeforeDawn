#pragma once

#include <Controllers/IController.h>
#include <Objects/Actors/Movable.h>

class RotatableController final : public IController {
public:
	explicit RotatableController(NotNull<Movable*> pMovable) noexcept;

	RotatableController() = delete;
	RotatableController(const RotatableController&) noexcept = default;
	RotatableController(RotatableController&&) noexcept = default;
	~RotatableController() noexcept override = default;

	RotatableController& operator = (const RotatableController&) noexcept = default;
	RotatableController& operator = (RotatableController&&) noexcept = default;

	void Update(const Float& deltaTime) override;

private:
	Movable* m_pMovable;

private:
	void Register(NotNull<Nt::Window*> pWindow) override;
	void Unregister(NotNull<Nt::Window*> pWindow) override;
};
