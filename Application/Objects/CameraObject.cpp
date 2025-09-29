#include <Objects/CameraObject.h>


CameraObject::CameraObject(GameObject* pTarget) noexcept :
	m_pTarget(pTarget)
{
}

void CameraObject::SetTarget(GameObject* pTarget) noexcept {
	m_pTarget = pTarget;
}

void CameraObject::Update() {
	if (m_pTarget == nullptr)
		return;

	SetPosition(-m_pTarget->GetPosition());
	SetAngle(m_pTarget->GetAngle());
}