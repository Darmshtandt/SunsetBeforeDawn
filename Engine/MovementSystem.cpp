#include <Engine/MovementSystem.h>
#include <World/Components/PhysicComponents.h>

void MovementSystem::RegisterObject(GameObject& object) {
	Movable movable;
	movable.pTransform = object.GetComponent<Transform3D>();
	movable.pMovement = object.GetComponent<Movement3D>();
	movable.pRigidBody = object.GetComponent<RigidBody>();
	movable.pObject = &object;

	if (movable.pTransform != nullptr && movable.pMovement != nullptr)
		m_Movables.emplace_back(movable);
}

void MovementSystem::UnregisterObject(const GameObject& object) {
	std::erase_if(m_Movables, [&object](const Movable& movable) noexcept {
		return movable.pObject == &object;
	});
}

void MovementSystem::Update(const Float& deltaTime) {
	for (const Movable& movable : m_Movables) {
		if (movable.pMovement->Direction.LengthSquare() != 0.f) {
			const Nt::Float3D velocity = movable.pMovement->Velocity();

			if (movable.pRigidBody == nullptr)
				movable.pTransform->Translate(velocity * deltaTime);
			else
				movable.pRigidBody->Body.AddForce(velocity * movable.pRigidBody->Body.GetMass());
			movable.pMovement->Direction = { };
		}

		if (movable.pMovement->DesiredRotation.LengthSquare() != 0.f) {
			movable.pTransform->Rotate(movable.pMovement->DesiredRotation * deltaTime);
			movable.pMovement->DesiredRotation = { };
		}
	}
}
