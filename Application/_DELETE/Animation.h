#pragma once

#include <StdH.h>

class Animation {
public:
	Animation(Nt::Sprite* pSprite, const Nt::Float2D& textureOffset, const Nt::Float2D& textureSize, const uInt& frameCount, const Double& delaySec) :
		m_pSprite(RequireNotNull(pSprite)),
		m_TextureOffset(textureOffset),
		m_TextureSize(textureSize),
		m_FrameCount(frameCount),
		m_DelayMs(uLLong(delaySec * 1000.0)),
		m_FrameID(0),
		m_IsPlayed(true)
	{
	}

	Void Update() {
		if (!m_IsPlayed)
			return;

		if (m_Timer.GetElapsedTimeMs() >= m_DelayMs) {
			m_Timer.Restart();

			Nt::FloatRect newTextureRect(m_TextureOffset, m_TextureOffset + m_TextureSize);

			const Float shiftValue = m_TextureSize.x * Float(m_FrameID);
			newTextureRect.Left += shiftValue;
			newTextureRect.Right += shiftValue;


			m_pSprite->SetTextureRect(newTextureRect);

			++m_FrameID;
			if (m_FrameID >= m_FrameCount)
				m_FrameID = 0;
		}
	}

	Void Play() noexcept {
		m_IsPlayed = true;
	}
	Void Pause() noexcept {
		m_IsPlayed = false;
	}
	Void Stop() noexcept {
		Pause();
		Reset();
	}

	Void Reset() noexcept {
		m_FrameID = 0;
	}

	[[nodiscard]]
	Bool IsPlayed() const noexcept {
		return m_IsPlayed;
	}

private:
	Nt::Float2D m_TextureOffset;
	Nt::Float2D m_TextureSize;
	Nt::Timer m_Timer;
	Nt::Sprite* m_pSprite;
	uLLong m_DelayMs;
	uInt m_FrameCount;
	uInt m_FrameID;
	Bool m_IsPlayed;
};