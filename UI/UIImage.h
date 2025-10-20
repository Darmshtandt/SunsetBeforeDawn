#pragma once

#include <World/Objects/GameObject.h>
#include <World/Components/UIComponents.h>

class TextureRenderer;

class UIImage final : public GameObject {
public:
	UIImage() noexcept;
	UIImage(const UIImage&) noexcept = default;
	UIImage(UIImage&&) noexcept = default;
	~UIImage() noexcept override = default;

	UIImage& operator = (const UIImage&) noexcept = default;
	UIImage& operator = (UIImage&&) noexcept = default;

public:
	Transform2D* pTransform;
	Layout2D* pLayout;
	Scale2D* pScale;
	TextureRenderer* pTexture;
};