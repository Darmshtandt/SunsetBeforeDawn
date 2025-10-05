#pragma once

#include <Nt/Graphics/Renderer.h>
#include <Nt/Graphics/RenderBuffer.h>
#include <Nt/Graphics/FrameBuffer.h>

class MSAA final {
public:
	MSAA() noexcept = default;
	MSAA(const MSAA&) noexcept = default;
	MSAA(MSAA&&) noexcept = default;
	~MSAA() noexcept = default;

	MSAA& operator = (const MSAA&) noexcept = default;
	MSAA& operator = (MSAA&&) noexcept = default;

	void Initialize(NotNull<Nt::Renderer*> pRenderer) noexcept;

	void Resize(const Nt::uIntRect& viewportRect);
	void BeginForRender();
	void ResolveToBuffer(Nt::FrameBuffer* pBuffer);

private:
	Nt::FrameBuffer m_FrameBuffer;
	Nt::RenderBuffer m_ColorBuffer;
	Nt::RenderBuffer m_DepthBuffer;
	Nt::uIntRect m_ViewportRect;
};