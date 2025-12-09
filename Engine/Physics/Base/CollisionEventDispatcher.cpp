#include <Engine/Physics/Base/CollisionEventDispatcher.h>
#include <World/Components/PhysicComponents.h>
#include <Core/BusLocator.h>

void CollisionEventDispatcher::ProcessFrameContacts(const std::vector<ObjectContactPair>& contacts) {
	for (const ObjectContactPair& pair : contacts) {
		const Collider* pColliderA = pair.pA->pCollider;
		const Collider* pColliderB = pair.pA->pCollider;

		if (m_PreviousContacts.contains(pair)) {
			if (pColliderA->OnStay)
				pColliderA->OnStay(*pair.pB, pair.Contact);

			if (pColliderB->OnStay)
				pColliderB->OnStay(*pair.pA, pair.Contact);

			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionStay>({ pair });
		}
		else {
			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionEnter>({ pair });
		}

		const auto it = std::find(m_PreviousContacts.cbegin(), m_PreviousContacts.cend(), pair);
		if (it != m_PreviousContacts.cend()) {
			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionExit>({ pair });
		}
	}

	for (const PhysicObjectPair& pair : m_PreviousContacts) {
		const auto it = std::find(contacts.cbegin(), contacts.cend(), pair);
		if (it != contacts.cend()) {
			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionExit>({ pair });
		}
	}

	m_PreviousContacts.clear();
	m_PreviousContacts.insert(contacts.begin(), contacts.end());
}