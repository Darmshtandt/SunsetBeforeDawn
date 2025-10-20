#pragma once

#include <World/Objects/GameObject.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/RenderComponents.h>
#include <Core/StdH.h>

class Arena final : public GameObject {
public:
	Arena() :
		GameObject(Class<Arena>::ID(), ObjectType::Static)
	{
		Transform3D* pTransform = AddComponent<Transform3D>();
		Collider* pCollider = AddComponent<Collider>();
		MeshRenderer* pMesh = AddComponent<MeshRenderer>();
		RigidBody* pRigidBody = AddComponent<RigidBody>();
		AddComponent<TextureRenderer>()->Set(TEXTURE_TEST);

		pCollider->SetShape(pMesh->Set(MESH_CUBE)->GetShape());
		pRigidBody->Body.SetMass(0.f);
		pTransform->Size({ 100.f, 1.f, 100.f });
	}

	Arena(const Arena&) noexcept = default;
	Arena(Arena&&) noexcept = default;
	~Arena() noexcept override = default;

	Arena& operator = (const Arena&) noexcept = default;
	Arena& operator = (Arena&&) noexcept = default;

private:
	inline static ObjectRegistrar<Arena> m_Registrar = { "Arena" };
};