// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Game.h>
#include <Objects/Actors/Player.h>
#include <Objects/Components/InputComponent.h>
#include <Objects/Components/MovementComponent.h>

Player::Player() noexcept :
	Character(Class<Player>::ID(), ObjectType::Player, "Humanoid")
{
	AddComponent<InputComponent>();
	AddComponent<MovementComponent>();

	GetBodyPtr()->SetColliderShape(
		Nt::ResourceManager::Instance().Get<Nt::Mesh>(MESH_CUBE)->GetShape());
}