#include <Engine/Physics/Base/CollisionEventDispatcher.h>
#include <Core/BusLocator.h>

void CollisionEventDispatcher::ProcessFrameContacts(const std::vector<CollisionContact>& contacts) {
	for (const CollisionContact& contact : contacts) {
		if (m_PreviousContacts.contains(contact)) {
			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionStay>({ contact });
		}
		else {
			BusLocator::PhysicsDispatcher()
				.Emmit<OnCollisionEnter>({ contact });
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