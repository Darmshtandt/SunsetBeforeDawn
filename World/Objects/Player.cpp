// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Objects/Player.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/PhysicComponents.h>
#include <World/Components/GameComponents.h>
#include <Core/StdH.h>

Player::Player() :
	Character(Class<Player>::ID(), ObjectType::Player, "Humanoid")
{
	AddComponent<Camera3D>()->Origin({ 0.f, 1.f, 0.f });
	AddComponent<Movement3D>()->Speed = 5.f;

	Nt::Mesh* pMesh = Nt::ResourceManager::Instance().Get<Nt::Mesh>(MESH_CUBE);
	GetComponent<Collider>()->SetShape(pMesh->GetShape());

	m_pHealth->SetMaxAmount(100.f);
	m_pHealth->Revive();
}

Player::Statistics Player::GetStatistics() const noexcept {
	Statistics stats = { };
	stats.HealthAmount = m_pHealth->GetAmount();
	stats.ArmorAmount = m_pArmor->GetAmount();
	stats.HealthMaxAmount = m_pHealth->GetMaxAmount();
	stats.ArmorMaxAmount = m_pArmor->GetMaxAmount();
	return stats;
}