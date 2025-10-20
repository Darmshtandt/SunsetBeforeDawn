#include <UI/HUD.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Core/StdH.h>

#include <World/Objects/Player.h>
#include <Core/BusLocator.h>

UIHealthBar::UIHealthBar() : Canvas(Class<UIHealthBar>::ID()) {
	m_pHealthBarLine = AddObject<UIImage>();
	m_pHealthBarLine->pScale->Padding({ 80.f, 28.f });
	m_pHealthBarLine->pScale->LocalPosition({ 0.5f, 0.5f });
	m_pHealthBarLine->pScale->LocalSize({ 2.f, 2.f });
	m_pHealthBarLine->pTexture->Set(TEXTURE_HP_BAR_LINE);

	UIImage* pHealthBar = AddObject<UIImage>();
	pHealthBar->pLayout->Padding = { 20.f, 20.f };
	pHealthBar->pScale->LocalPosition({ 0.5f, 0.5f });
	pHealthBar->pScale->LocalSize({ 2.f, 2.f });
	pHealthBar->pTexture->Set(TEXTURE_HP_BAR);

	AddComponent<Handler>()->Function = [this] (const Float& deltaTime) {
		if (!BusLocator::StatisticsQuery().Has<Player::Statistics>())
			return;

		const auto statistics = BusLocator::StatisticsQuery().Request<Player::Statistics>();
		if (statistics.HealthMaxAmount == 0.f)
			return;

		const Float percentage =
			statistics.HealthAmount / statistics.HealthMaxAmount;
		m_pHealthBarLine->pScale->LocalSize({ percentage * 2.f, 2.f });
		};
}

UIArmorBar::UIArmorBar() : Canvas(Class<UIArmorBar>::ID()) {
	UIImage* pArmorBar = AddObject<UIImage>();
	pArmorBar->pLayout->Padding = { 20.f, 20.f };
	pArmorBar->pScale->LocalPosition({ 0.5f, 1.6f });
	pArmorBar->pScale->LocalSize({ 2.f, 2.f });
	pArmorBar->pTexture->Set(TEXTURE_ARMOR_BAR);
}