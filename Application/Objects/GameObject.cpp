// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Objects/GameObject.h>

GameObject::GameObject(const ClassID& id, const ObjectType& type) noexcept :
	Identifier(id),
	m_Type(type)
{
}

ObjectType GameObject::GetType() const noexcept {
	return m_Type;
}