// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Objects/GameObject.h>

GameObject::GameObject(const ClassID& id, const ObjectType& type) noexcept :
	Identifier(id),
	m_Type(type)
{
}

void GameObject::UnmarkValid() noexcept {
	m_IsValid = false;
}

ObjectType GameObject::GetType() const noexcept {
	return m_Type;
}

Bool GameObject::IsValid() const noexcept {
	return m_IsValid;
}