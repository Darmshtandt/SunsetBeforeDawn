// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Objects/Actors/Player.h>
#include <Objects/Components/RenderComponents.h>
#include <Objects/Components/PhysicComponents.h>
#include <Objects/Components/GameComponents.h>
#include <StdH.h>

Player::Player() :
	Character(Class<Player>::ID(), ObjectType::Player, "Humanoid")
{
	AddComponent<InputComponent>();
	AddComponent<Camera3D>();
	AddComponent<Movement3D>()->Speed = 5.f;

	Nt::Mesh* pMesh = Nt::ResourceManager::Instance().Get<Nt::Mesh>(MESH_CUBE);
	GetComponent<ColliderComponent>()->Collider
		.SetShape(pMesh->GetShape());
}

void Player::Update(const Float& deltaTime) {
	(void)deltaTime;
}

Player::Statistics Player::GetStatistics() const noexcept {
	Statistics stats = { };
	stats.HealthAmount = m_pHealth->GetAmount();
	stats.ArmorAmount = m_pArmor->GetAmount();
	return stats;
}