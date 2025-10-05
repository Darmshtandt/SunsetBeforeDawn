#pragma once

#include <Objects/Components/BasicComponents.h>
#include <Physics/RigidBody.h>
#include <Nt/Collider.h>

struct RigidBodyComponent final : public IComponent {
	explicit RigidBodyComponent(GameObject* pOwner) :
		IComponent(pOwner) {
	}

	RigidBody Body;
};

struct ColliderComponent final : public IComponent {
	explicit ColliderComponent(GameObject* pOwner) :
		IComponent(pOwner) {
	}

	Nt::Collider Collider;
};