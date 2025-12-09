#pragma once

#include <Engine/IController.h>
#include <World/Components/RenderComponents.h>
#include <World/Objects/GameObject.h>

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
	Transform3D* m_pObjectTransform;
	Camera3D* m_pCamera;
};