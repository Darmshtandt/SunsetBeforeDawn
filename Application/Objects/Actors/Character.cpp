#include <Objects/Actors/Character.h>
#include <Physics/RigidBody.h>

Character::Character(const ClassID& id, const ObjectType& type, std::string factionName) noexcept :
	GameObject(id, type),
	Actor(Health(0.f, 0.f), Armor(0.f, 0.f)),
	m_FactionName(std::move(factionName))
{
	SetBody<RigidBody>();
}

void Character::Move(const Nt::Float3D& velocity) {
	Translate(velocity);
}

void Character::ApplyRotation(const Nt::Float3D& angle) {
	Rotate(angle);
}

Float Character::GetSpeed() const noexcept {
	return m_Speed;
}

Float Character::GetPitch() const noexcept {
	return m_Angle.y;
}

std::string Character::GetFactionName() const noexcept {
	return m_FactionName;
}

void Character::SetSpeed(const Float& speed) noexcept {
	m_Speed = speed;
}
