#pragma once

#include <World/Components/BasicComponents.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>
#include <Nt/Graphics/System/FTFont.h>
#include <unordered_map>

struct ICamera {
	virtual ~ICamera() noexcept = default;

	[[nodiscard]] virtual Nt::Matrix4x4 View() const noexcept = 0;
	[[nodiscard]] virtual Bool IsDirty() const noexcept = 0;
	[[nodiscard]] virtual uInt GetDimension() const noexcept = 0;
};

template <typename _Ty, uInt Dimension>
class Camera final : public IComponent, public ICamera {
public:
	using Transform = TransformSelector<_Ty, Dimension>::Type;
	using Vector = typename Transform::Vector;

public:
	explicit Camera(GameObject* pOwner) :
		IComponent(pOwner, Class<Camera>::ID()),
		m_LocalTransform(pOwner)
	{
		m_pParentTransform = pOwner->GetComponent<Transform>();
		if (m_pParentTransform == nullptr)
			m_pParentTransform = pOwner->AddComponent<Transform>();
		m_LocalTransform.SetParent(m_pParentTransform);
	}
	~Camera() noexcept override = default;

	[[nodiscard]] Nt::Matrix4x4 View() const noexcept override {
		return m_LocalTransform.WorldToLocal();
	}

	[[nodiscard]] Transform& LocalTransform() noexcept {
		return m_LocalTransform;
	}
	[[nodiscard]] uInt GetDimension() const noexcept override {
		return Dimension;
	}
	[[nodiscard]] Bool IsDirty() const noexcept override {
		return m_pParentTransform->IsDirty();
	}

private:
	Transform* m_pParentTransform;
	Transform m_LocalTransform;
};

template <class _Ty, Requires(std::is_base_of_v<Nt::IResource, _Ty>)>
class ResourceRenderer {
public:
	ResourceRenderer() noexcept = default;
	ResourceRenderer(const ResourceRenderer&) noexcept = default;
	ResourceRenderer(ResourceRenderer&&) noexcept = default;
	virtual ~ResourceRenderer() noexcept = default;

	ResourceRenderer& operator = (const ResourceRenderer&) noexcept = default;
	ResourceRenderer& operator = (ResourceRenderer&&) noexcept = default;

	[[nodiscard]] const _Ty* Get() const noexcept {
		return m_Resource.Get();
	}
	[[nodiscard]] Bool IsValid() const noexcept {
		return m_Resource.IsValid();
	}

	const _Ty* SetByPointer(_Ty* pointer) noexcept {
		return m_Resource.SetByPointer(pointer);
	}
	const _Ty* Set(const uInt& index) noexcept {
		return m_Resource.Set(index);
	}

private:
	Nt::ResourceHandle<_Ty> m_Resource;
};

class MeshRenderer final : public IComponent, public ResourceRenderer<Nt::Mesh> {
public:
	explicit MeshRenderer(GameObject* pOwner) : IComponent(pOwner, Class<MeshRenderer>::ID())
	{
	}
	~MeshRenderer() noexcept override = default;

public:
	Bool IsVisible = true;
};

class TextureRenderer final : public IComponent, public ResourceRenderer<Nt::Texture> {
public:
	explicit TextureRenderer(GameObject* pOwner) :
		IComponent(pOwner, Class<TextureRenderer>::ID())
	{
	}
	~TextureRenderer() noexcept override = default;
};

class TextureMap final : public IComponent {
public:
	explicit TextureMap(GameObject* pOwner) :
		IComponent(pOwner, Class<TextureMap>::ID())
	{
	}

	Nt::Texture Main;
	std::unordered_map<uInt, Nt::Texture> Map;
};

class Text final : public IComponent {
public:
	explicit Text(GameObject* pOwner) :
		IComponent(pOwner, Class<Text>::ID())
	{
	}

	std::string TextString;
	Nt::FTFont Font;
};

using Camera2D = Camera<Float, 2>;
using Camera3D = Camera<Float, 3>;