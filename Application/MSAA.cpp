#include <MSAA.h>

void MSAA::Initialize(NotNull<Nt::Renderer*> pRenderer) noexcept {
	pRenderer->EnableMultisample();
	pRenderer->Hint(Nt::HintTarget::MULTISAMPLE_FILTER_NV, Nt::HintMode::NICEST);

	m_FrameBuffer.Create();
	if (m_FrameBuffer.GetStatus() != Nt::FrameBuffer::Status::COMPLETE)
		Raise("Failed to create FrameBuffer");
}

void MSAA::Resize(const Nt::uIntRect& viewportRect) {
	if (m_ViewportRect == viewportRect)
		return;

	m_ViewportRect = viewportRect;
	m_FrameBuffer.Bind(Nt::FrameBuffer::Type::DEFAULT);

	if (m_ColorBuffer.GetID() != 0)
		m_ColorBuffer.Delete();
	if (m_DepthBuffer.GetID() != 0)
		m_DepthBuffer.Delete();

	m_ColorBuffer.Create();
	m_ColorBuffer.StorageMultisample(16, Nt::InternalFormat::RGBA8, viewportRect.RightBottom);
	m_FrameBuffer.SetRenderBuffer(m_ColorBuffer, Nt::Attachment::COLOR_0);

	if (m_FrameBuffer.GetStatus() != Nt::FrameBuffer::Status::COMPLETE)
		Raise("Failed to set ColorBuffer");

	m_DepthBuffer.Create();
	m_DepthBuffer.StorageMultisample(16, Nt::InternalFormat::DEPTH24_STENCIL8, viewportRect.RightBottom);
	m_FrameBuffer.SetRenderBuffer(m_DepthBuffer, Nt::Attachment::DEPTH);

	if (m_FrameBuffer.GetStatus() != Nt::FrameBuffer::Status::COMPLETE)
		Raise("Failed to set DepthBuffer");

	m_FrameBuffer.Unbind(Nt::FrameBuffer::Type::DEFAULT);
}

void MSAA::BeginForRender() {
	m_FrameBuffer.Bind(Nt::FrameBuffer::Type::DRAW);
}

void MSAA::ResolveToBuffer(Nt::FrameBuffer* pBuffer) {
	if (pBuffer != nullptr)
		pBuffer->Bind(Nt::FrameBuffer::Type::DRAW);
	else
		m_FrameBuffer.Unbind(Nt::FrameBuffer::Type::DRAW);

	m_FrameBuffer.Bind(Nt::FrameBuffer::Type::READ);
	m_FrameBuffer.Blit(m_ViewportRect, m_ViewportRect,
		Nt::BufferBit::COLOR, Nt::MAG_NEAREST);

	m_FrameBuffer.Unbind(Nt::FrameBuffer::Type::DEFAULT);
}