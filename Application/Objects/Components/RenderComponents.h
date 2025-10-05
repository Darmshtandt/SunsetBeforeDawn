#pragma once

#include <Objects/Components/BasicComponents.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>
#include <unordered_map>

struct ICamera {
	virtual ~ICamera() noexcept = default;

	[[nodiscard]] virtual Nt::Matrix4x4 View() const noexcept = 0;
	[[nodiscard]] virtual Bool IsDirty() const noexcept = 0;
	[[nodiscard]] virtual Bool Is2D() const noexcept = 0;
	[[nodiscard]] virtual Bool Is3D() const noexcept = 0;
};

template <typename _Ty, uInt Dimension>
class Camera final : public IComponent, public ICamera {
public:
	using Transform = Transform<_Ty, Dimension>;

public:
	explicit Camera(GameObject* pOwner) : IComponent(pOwner) {
		pTransform = pOwner->GetComponent<Transform>();
		if (pTransform == nullptr)
			pTransform = pOwner->AddComponent<Transform>();;
	}
	~Camera() noexcept override = default;

	[[nodiscard]] Nt::Matrix4x4 View() const noexcept override {
		Nt::Matrix4x4 view = pTransform->WorldToLocal();
		view.Rows[3].xyz *= -1;
		return view;
	}

	[[nodiscard]] Bool IsDirty() const noexcept override {
		return pTransform->IsDirty();
	}
	[[nodiscard]] Bool Is2D() const noexcept override {
		return Dimension == 2;
	}
	[[nodiscard]] Bool Is3D() const noexcept override {
		return Dimension == 3;
	}

public:
	Transform* pTransform;
};

struct MeshComponent final : public IComponent {
	explicit MeshComponent(GameObject* pOwner) : IComponent(pOwner)
	{
	}

	Nt::ResourceHandle<Nt::Mesh> Handler;
	Bool IsVisible = true;
};

struct TextureComponent final : public IComponent {
	explicit TextureComponent(GameObject* pOwner) : IComponent(pOwner)
	{
	}

	Nt::ResourceHandle<Nt::Texture> Handler;
};

struct TextureMapComponent final : public IComponent {
	explicit TextureMapComponent(GameObject* pOwner) : IComponent(pOwner)
	{
	}

	Nt::Texture Main;
	std::unordered_map<uInt, Nt::Texture> Map;
};

using Camera2D = Camera<Float, 2>;

using Camera3D = Camera<Float, 3>;