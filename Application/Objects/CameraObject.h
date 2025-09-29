#pragma once

#include <Objects/GameObject.h>
#include <Nt/Graphics/Objects/Camera.h>

class CameraObject : public Nt::Camera {
public:
	explicit CameraObject(GameObject* pTarget) noexcept;

	CameraObject() = delete;
	CameraObject(const CameraObject&) noexcept = default;
	CameraObject(CameraObject&&) noexcept = default;
	~CameraObject() noexcept override = default;

	CameraObject& operator = (const CameraObject&) noexcept = default;
	CameraObject& operator = (CameraObject&&) noexcept = default;

	void SetTarget(GameObject* pTarget) noexcept;
	void Update();

private:
	GameObject* m_pTarget;
};