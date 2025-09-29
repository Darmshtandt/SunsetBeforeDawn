#include <Physics/PhysicsSystem.h>
#include <Physics/StaticBody.h>

void PhysicsSystem::Update(const Float& deltaTime) const noexcept {
	for (IBody* pBody : m_Bodies) {
		if (Class<RigidBody>::Is(*pBody)) {
			RigidBody* pRigid = static_cast<RigidBody*>(pBody);

			if (m_EnabledGravitation)
				pRigid->AddForce(m_GravityDirection * deltaTime);
			pBody->Update(deltaTime);
		}
	}
}

void PhysicsSystem::AddObject(NotNull<GameObject*> pObject) {
	IBody* pBody = pObject->GetBodyPtr();
	if (pBody == nullptr)
		return;

	m_Bodies.emplace_back(pBody);

	if (!Class<StaticBody>::Is(*pBody)) {
		pObject->SubscribeOnMoved([this] (GameObject* pObject) {
			IBody* pThisBody = pObject->GetBodyPtr();

			for (const IBody* pBody : m_Bodies) {
				if (!Class<StaticBody>::Is(*pBody) && pBody != pThisBody)
					pThisBody->HandleCollision(pBody);
			}
			});
	}
}

void PhysicsSystem::RemoveObject(NotNull<const GameObject*> pObject) {
	const IBody* pBody = pObject->GetBodyPtr();
	if (pBody != nullptr)
		std::erase(m_Bodies, pBody);
}

void PhysicsSystem::Clear() {
	m_Bodies.clear();
}