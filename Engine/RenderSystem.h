#pragma once

#include <Core/MSAA.h>
#include <World/Objects/GameObject.h>

class Collider;
struct ICamera;
class MeshRenderer;
class TextureRenderer;
class Text;

class RenderSystem final {
private:
	enum Projection : Byte {
		NONE = -1,
		PERSPECTIVE,
		ORTHO2D,
		ORTHO,
	};

	struct Renderable final {
		const Transform2D* pTransform2D = nullptr;
		const Transform3D* pTransform3D = nullptr;
		const MeshRenderer* pMesh = nullptr;
		const TextureRenderer* pTexture = nullptr;
		const Text* pText = nullptr;
		const Collider* pCollider = nullptr;
		const GameObject* pObject = nullptr;
	};

public:
	RenderSystem(NotNull<Nt::Renderer*> pRenderer);

	RenderSystem() = delete;
	RenderSystem(const RenderSystem&) noexcept = default;
	RenderSystem(RenderSystem&&) noexcept = default;
	~RenderSystem() noexcept = default;

	RenderSystem& operator = (const RenderSystem&) noexcept = default;
	RenderSystem& operator = (RenderSystem&&) noexcept = default;

	void RegisterObject(const GameObject& object);
	void UnregisterObject(const GameObject& object);
	void ClearObjects() noexcept;

	void Resize(const Nt::Float2D& size);
	void ClearScreen();

	void UpdateCamera();
	void RenderScene();
	void RenderUI();
	void RenderObject(const Renderable& renderable);
	void RenderCollider(const Renderable& renderable);
	void RenderText(const Renderable& renderable);

	void SetActiveProjection(const Projection& projection);

private:
	const ICamera* m_pCamera = nullptr;
	std::vector<Renderable> m_SceneObject;
	std::vector<Renderable> m_UIObject;
	Nt::Shader m_TerrainShader;
	Nt::Shader m_pUIShader;
	Nt::Renderer* m_pRenderer;
	MSAA m_MSAA;
	Nt::Matrix4x4 m_Projections[3];
	Projection m_ActiveProjection = NONE;
};