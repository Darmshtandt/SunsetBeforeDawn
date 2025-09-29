#include <Controllers/RotatableController.h>

RotatableController::RotatableController(NotNull<Movable*> pMovable) noexcept :
	m_pMovable(pMovable)
{
}

void RotatableController::Update(const Float& deltaTime) {
	if (!IsEnabled())
		return;

	const Nt::Float2D cursorPosition = Nt::GetCursorPosition();
	const Nt::Float2D screenCenter(Nt::GetMonitorSize() / 2);

	if (!(cursorPosition == screenCenter)) {
		const Nt::Float2D angleRotation = (cursorPosition - screenCenter) / -6.f;

		m_pMovable->ApplyRotation({ angleRotation.y, angleRotation.x, 0.f });
		Nt::Mouse::SetCursorPosition(screenCenter);
	}
}

void RotatableController::Register(NotNull<Nt::Window*> pWindow)
{
}
void RotatableController::Unregister(NotNull<Nt::Window*> pWindow)
{
}