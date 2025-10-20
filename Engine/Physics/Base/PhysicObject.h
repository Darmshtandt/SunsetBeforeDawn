#pragma once

#include <World/Objects/GameObject.h>

class RigidBody;
class Collider;

struct PhysicObject {
	Transform3D* pTransform = nullptr;
	RigidBody* pRigidBody = nullptr;
	Collider* pCollider = nullptr;
	GameObject* pObject = nullptr;
};