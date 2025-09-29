#include <Controllers/PlayerController.h>

PlayerController::PlayerController(NotNull<Player*> pPlayer) :
	m_pPlayer(pPlayer)
{
	Assert(pPlayer->HasComponent<InputComponent>(), "InputComponent is missing");
	Assert(pPlayer->HasComponent<MovementComponent>(), "MovementComponent is missing");

	m_pInputComponent = pPlayer->GetComponent<InputComponent>();
	m_pMovementComponent = pPlayer->GetComponent<MovementComponent>();
}

void PlayerController::Update(const Float& deltaTime) {
	m_pMovementComponent->DesiredVelocity =
		GetDirection() * m_pPlayer->GetSpeed() * deltaTime;
}

Nt::Float3D PlayerController::GetDirection() const noexcept {
	const std::unordered_set<Command>& commands = m_pInputComponent->Commands;
	const Float& pitch = m_pPlayer->GetPitch();

	Nt::Float3D direction;
	if (commands.contains(Command::Left)) {
		direction.x += sinf((pitch - 90.f) * RADf);
		direction.z += cosf((pitch - 90.f) * RADf);
	}
	if (commands.contains(Command::Right)) {
		direction.x += sinf((pitch + 90.f) * RADf);
		direction.z += cosf((pitch + 90.f) * RADf);
	}

	if (commands.contains(Command::Up))
		direction.y += 1.f;
	if (commands.contains(Command::Down))
		direction.y -= 1.f;

	if (commands.contains(Command::Forward)) {
		direction.x -= sinf(pitch * RADf);
		direction.z -= cosf(pitch * RADf);
	}
	if (commands.contains(Command::Back)) {
		direction.x += sinf(pitch * RADf);
		direction.z += cosf(pitch * RADf);
	}

	return direction.GetNormalize();
}
