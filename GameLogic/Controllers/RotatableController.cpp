#include <GameLogic/Controllers/RotatableController.h>
#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Core/Input.h>

RotatableController::RotatableController(NotNull<GameObject*> pObject) :
	m_pObjectTransform(pObject->GetComponent<Transform3D>()),
	m_pCamera(pObject->GetComponent<Camera3D>())
{
	RequireNotNull(m_pObjectTransform);
	RequireNotNull(m_pCamera);
}

void RotatableController::Update(const Float& deltaTime) {
	if (!IsEnabled())
		return;

	const Nt::Float2D cursorPosition = Nt::GetCursorPosition();
	const Nt::Float2D screenCenter(Nt::GetMonitorSize() / 2);

	if (!(cursorPosition == screenCenter)) {
		const Nt::Float2D rotation = -(cursorPosition - screenCenter) * 10.f * RADf * deltaTime;

		m_pObjectTransform->Rotate({ 0.f, rotation.x, 0.f });
		m_pCamera->LocalTransform().Rotate({ rotation.y, 0.f, 0.f });
		Nt::Mouse::SetCursorPosition(screenCenter);
	}
}