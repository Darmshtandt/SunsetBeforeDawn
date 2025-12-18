#pragma once

#include <World/Objects/GameObject.h>
#include <World/Components/PhysicComponents.h>
#include <Core/BusLocator.h>
#include <Engine/Physics/Base/CollisionEventDispatcher.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/CombatComponents.h>
#include <Engine/Combat/CombatInterfaces.h>
#include <Engine/Combat/DamageLogic.h>
#include <Core/StdH.h>

class Bomb : public GameObject {
public:
	struct OnExplosion final {
		Transform3D* pBombTransform;
		IDamage* pDamage;
	};

public:
	Bomb() : GameObject(Class<Bomb>::ID(), ObjectType::Other) {
		auto* pTransform = AddComponent<Transform3D>();
		auto* pMesh = AddComponent<MeshRenderer>();
		auto* pCombat = AddComponent<Combat>();
		AddComponent<TextureRenderer>()->Set(TEXTURE_TEST);

		pMesh->Set(MESH_BOMB);
		AddComponent<Collider>()->SetShape(pMesh->Get()->GetShape());

		m_SphereDamage.Radius = 10.f;
		m_SphereDamage.Amount = 10.f;
		pCombat->SetDamageType(&m_SphereDamage);

		m_Sub = BusLocator::PhysicsDispatcher().Subscribe<OnCollisionEnter>(
			[this, pTransform, pCombat](const OnCollisionEnter& event) {
				if (event.Contact.pA->pObject != this && event.Contact.pB->pObject != this)
					return;

				pCombat->PerformAttack(pTransform->Position(), pTransform->Rotation());
				UnmarkValid();
			});
	}
	~Bomb() noexcept {
		BusLocator::PhysicsDispatcher().Unsubscribe<OnCollisionEnter>(m_Sub);
	}
	
private:
	Subscription m_Sub;
	SphereDamage m_SphereDamage;
	inline static ObjectRegistrar<Bomb> m_Registrar = { "Bomb" };
};