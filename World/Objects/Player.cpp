// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Objects/Player.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/GameComponents.h>
#include <Core/StdH.h>
#include <World/Components/CombatComponents.h>
#include <World/Objects/Weapons/Pistol.h>

Player::Player() :
	Character(Class<Player>::ID(), ObjectType::Player, "Human")
{
	m_pCamera = AddComponent<Camera3D>();
	m_pCamera->LocalTransform().LocalPosition({ 0.f, 1.f, 0.f });
	m_pMovement->Speed = 25.f;

	AddComponent<Combat>();
	AddComponent<WeaponBelt>()->Add<Pistol>();

	Nt::Mesh* pMesh = Nt::ResourceManager::Instance().Get<Nt::Mesh>(MESH_CUBE);
	m_pCollider->SetShape(pMesh->GetShape());

	m_pHealth->SetMaxAmount(100.f);
	m_pHealth->Revive();
}

Nt::Float3D Player::GetFullRotation() const noexcept {
	return m_pCamera->LocalTransform().Rotation();
}

Player::Statistics Player::GetStatistics() const noexcept {
	Statistics stats = { };
	stats.HealthAmount = m_pHealth->GetAmount();
	stats.ArmorAmount = m_pArmor->GetAmount();
	stats.HealthMaxAmount = m_pHealth->GetMaxAmount();
	stats.ArmorMaxAmount = m_pArmor->GetMaxAmount();
	return stats;
}
