#pragma once

#include <UI/Canvas.h>
#include <World/Components/UIComponents.h>

class TextureRenderer;
class Text;

class UISystem final {
public:
	struct UIElement final {
		Transform2D* pTransform = nullptr;
		Scale2D* pScale = nullptr;
		TextureRenderer* pTexture = nullptr;
		Text* pText = nullptr;
		GameObject* pObject = nullptr;
	};

public:
	UISystem() noexcept = default;
	UISystem(const UISystem&) noexcept = default;
	UISystem(UISystem&&) noexcept = default;
	~UISystem() noexcept = default;

	UISystem& operator = (const UISystem&) noexcept = default;
	UISystem& operator = (UISystem&&) noexcept = default;

	void Resize(const Nt::Float2D& contextSize);

	void RegisterCanvas(Canvas& canvas);
	void UnregisterCanvas(const Canvas& canvas);
	void Clear() noexcept;

	void Update(const Float& deltaTime);

private:
	std::vector<UIElement> m_Elements;
	Nt::Float2D m_ContextSize;
};