#pragma once

#include <unordered_set>
#include <Engine/Physics/Base/PhysicsInterfaces.h>

struct OnCollisionEnter final {
	ObjectContactPair Contact;
};
struct OnCollisionStay final {
	ObjectContactPair Contact;
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

	void ProcessFrameContacts(const std::vector<ObjectContactPair>& contacts);

private:
	std::unordered_set<PhysicObjectPair, PhysicObjectHash> m_PreviousContacts;
};