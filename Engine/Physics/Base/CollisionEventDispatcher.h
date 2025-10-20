#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>

struct OnCollisionEnter final {
	CollisionContact Contact;
};
struct OnCollisionStay final {
	CollisionContact Contact;
};
struct OnCollisionExit final {
	PhysicObjectPair Pair;
};

class CollisionEventDispatcher final {
public:
	CollisionEventDispatcher() noexcept = default;
	CollisionEventDispatcher(const CollisionEventDispatcher&) = delete;
	CollisionEventDispatcher(CollisionEventDispatcher&&) noexcept = default;
	~CollisionEventDispatcher() noexcept = default;

	CollisionEventDispatcher& operator = (const CollisionEventDispatcher&) = delete;
	CollisionEventDispatcher& operator = (CollisionEventDispatcher&&) noexcept = default;

	void ProcessFrameContacts(const std::vector<CollisionContact>& contacts);

private:
	std::unordered_set<PhysicObjectPair, PhysicObjectHash> m_PreviousContacts;
};