#pragma once

#include <World/Objects/GameObject.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/UIComponents.h>

class UIText final : public GameObject {
public:
	UIText();
	UIText(const UIText&) noexcept = default;
	UIText(UIText&&) noexcept = default;
	~UIText() noexcept override = default;

	UIText& operator = (const UIText&) noexcept = default;
	UIText& operator = (UIText&&) noexcept = default;

public:
	Transform2D* pTransform;
	Scale2D* pScale;
	Text* pText;
};