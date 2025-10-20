#include <GameLogic/Controllers/RotatableController.h>
#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Core/Input.h>

RotatableController::RotatableController(NotNull<GameObject*> pObject) :
	m_pMovement(pObject->GetComponent<Movement3D>())
{
	RequireNotNull(m_pMovement);
}

void RotatableController::Update(const Float& deltaTime) {
	if (!IsEnabled())
		return;

	const Nt::Float2D cursorPosition = Nt::GetCursorPosition();
	const Nt::Float2D screenCenter(Nt::GetMonitorSize() / 2);

	if (!(cursorPosition == screenCenter)) {
		const Nt::Float2D rotation = (cursorPosition - screenCenter) * 10.f * RADf;

		m_pMovement->DesiredRotation.x = rotation.y;
		m_pMovement->DesiredRotation.y = rotation.x;

		Nt::Mouse::SetCursorPosition(screenCenter);
	}
}