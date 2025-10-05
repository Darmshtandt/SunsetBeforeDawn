#pragma once

#include <Objects/GameObject.h>
#include <Objects/Components/UIComponents.h>

class TextureComponent;

class UIImage final : public GameObject {
public:
	UIImage() noexcept;
	UIImage(const UIImage&) noexcept = default;
	UIImage(UIImage&&) noexcept = default;
	~UIImage() noexcept override = default;

	UIImage& operator = (const UIImage&) noexcept = default;
	UIImage& operator = (UIImage&&) noexcept = default;

	void Update(const Float& deltaTime) override;

public:
	Transform2D* pTransform;
	Layout2D* pLayout;
	Scale2D* pScale;
	TextureComponent* pTexture;
};