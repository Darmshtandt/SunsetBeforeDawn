// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Objects/GameObject.h>

GameObject::GameObject(const ClassID& id, const ObjectType& type) noexcept :
	Identifier(id),
	m_Type(type)
{
}

void GameObject::Render(NotNull<Nt::Renderer*> pRenderer) const {
	Model::Render(pRenderer);
	if (m_pBody)
		;// m_pBody->GetCollider().Render(pRenderer);
}

void GameObject::Translate(const Nt::Float3D& offset) noexcept {
	Model::Translate(offset);
	m_OnMoved.Emmit(this);
}

void GameObject::SubscribeOnMoved(const Signal<GameObject*>::Slot& onMoved) {
	m_OnMoved.Connect(onMoved);
}

Float GameObject::GetDistance(const GameObject& object) const noexcept {
	return m_Position.GetDistance(object.m_Position);
}

ObjectType GameObject::GetType() const noexcept {
	return m_Type;
}

IBody* GameObject::GetBodyPtr() noexcept {
	return m_pBody.get();
}

const IBody* GameObject::GetBodyPtr() const noexcept {
	return m_pBody.get();
}