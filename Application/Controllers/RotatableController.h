#pragma once

#include <Controllers/IController.h>
#include <Objects/GameObject.h>

class RotatableController final : public IController {
public:
	explicit RotatableController(NotNull<GameObject*> pObject);

	RotatableController() = delete;
	RotatableController(const RotatableController&) noexcept = default;
	RotatableController(RotatableController&&) noexcept = default;
	~RotatableController() noexcept override = default;

	RotatableController& operator = (const RotatableController&) noexcept = default;
	RotatableController& operator = (RotatableController&&) noexcept = default;

	void Update(const Float& deltaTime) override;

private:
	Movement3D* m_pMovement;
};