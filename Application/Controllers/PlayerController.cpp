#include <Controllers/PlayerController.h>

PlayerController::PlayerController(NotNull<Player*> pPlayer) :
	m_pTransform(pPlayer->GetComponent<Transform3D>()),
	m_pMovement(pPlayer->GetComponent<Movement3D>()),
	m_pInput(pPlayer->GetComponent<InputComponent>())
{
	Assert(m_pTransform != nullptr, "TransformComponent is missing");
	Assert(m_pMovement != nullptr, "MovementComponent is missing");
	Assert(m_pInput != nullptr, "InputComponent is missing");
}

void PlayerController::Update(const Float& deltaTime) {
	m_pMovement->Direction = GetDirection();
}

Nt::Float3D PlayerController::GetDirection() const noexcept {
	const std::unordered_set<Command>& commands = m_pInput->Commands;
	const Float& pitch = m_pTransform->Rotation().y;

	Nt::Float3D direction;
	if (commands.contains(Command::Left)) {
		direction.x += sinf(pitch - 90.f);
		direction.z += cosf(pitch - 90.f);
	}
	if (commands.contains(Command::Right)) {
		direction.x += sinf(pitch + 90.f);
		direction.z += cosf(pitch + 90.f);
	}

	if (commands.contains(Command::Up))
		direction.y += 1.f;
	if (commands.contains(Command::Down))
		direction.y -= 1.f;

	if (commands.contains(Command::Forward)) {
		direction.x -= sinf(pitch);
		direction.z -= cosf(pitch);
	}
	if (commands.contains(Command::Back)) {
		direction.x += sinf(pitch);
		direction.z += cosf(pitch);
	}

	return direction.GetNormalize();
}
