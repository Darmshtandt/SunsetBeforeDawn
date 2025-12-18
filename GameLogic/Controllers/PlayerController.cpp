#include <GameLogic/Controllers/PlayerController.h>
#include <Core/BusLocator.h>
#include <Core/Input/Commands.h>

#include <iostream>

PlayerController::PlayerController(NotNull<Player*> pPlayer) :
	m_pTransform(pPlayer->GetComponent<Transform3D>()),
	m_pMovement(pPlayer->GetComponent<Movement3D>())
{
	Assert(m_pTransform != nullptr, "TransformComponent is missing");
	Assert(m_pMovement != nullptr, "MovementComponent is missing");

	m_ActionSubscription = BusLocator::ActionDispatcher()
		.Subscribe<InputCommand>([this] (const InputCommand& input) {
			if (input.ContextID == InputContextID::Movement)
				Execute(input.Commands);
		});
}
PlayerController::~PlayerController() {
	BusLocator::ActionDispatcher().Unsubscribe<InputCommand>(m_ActionSubscription);
}

void PlayerController::Execute(const std::unordered_set<InputCommandID>& commands) const noexcept {
	Nt::Float3D direction;
	if (commands.contains(InputCommandID::Left))
		direction -= m_pTransform->Right();
	if (commands.contains(InputCommandID::Right))
		direction += m_pTransform->Right();

	if (commands.contains(InputCommandID::Up))
		direction.y += 1.f;
	if (commands.contains(InputCommandID::Down))
		direction.y -= 1.f;

	if (commands.contains(InputCommandID::Forward))
		direction += m_pTransform->Forward();
	if (commands.contains(InputCommandID::Back))
		direction -= m_pTransform->Forward();

	m_pMovement->Direction = direction.GetNormalize();

	//constexpr Float halfPI = PIf / 2.f;
	//const Float& pitch = -m_pTransform->Rotation().y;

	//std::cout << m_pTransform->LocalRotation().ToString() << ' ' << m_pTransform->Rotation().ToString() << std::endl;

	//Nt::Float3D direction;
	//if (commands.contains(InputCommandID::Left)) {
	//	direction.x += sinf(pitch - halfPI);
	//	direction.z += cosf(pitch - halfPI);
	//}
	//if (commands.contains(InputCommandID::Right)) {
	//	direction.x += sinf(pitch + halfPI);
	//	direction.z += cosf(pitch + halfPI);
	//}

	//if (commands.contains(InputCommandID::Up))
	//	direction.y += 1.f;
	//if (commands.contains(InputCommandID::Down))
	//	direction.y -= 1.f;

	//if (commands.contains(InputCommandID::Forward)) {
	//	direction.x += sinf(pitch);
	//	direction.z += cosf(pitch);
	//}
	//if (commands.contains(InputCommandID::Back)) {
	//	direction.x -= sinf(pitch);
	//	direction.z -= cosf(pitch);
	//}

	//m_pMovement->Direction = direction.GetNormalize();
}