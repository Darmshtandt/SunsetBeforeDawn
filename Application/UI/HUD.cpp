#include <UI/HUD.h>
#include <UI/UIImage.h>
#include <Objects/Components/UIComponents.h>
#include <Objects/Components/RenderComponents.h>
#include <StdH.h>

HealthBar::HealthBar() : Canvas(Class<HealthBar>::ID()) {
	UIImage* pHealthBar = AddObject<UIImage>();
	pHealthBar->pLayout->Padding = { 20.f, 20.f };
	pHealthBar->pScale->LocalPosition({ 0.5f, 0.5f });
	pHealthBar->pScale->LocalSize({ 2.f, 2.f });
	pHealthBar->pTexture->Handler.Set(TEXTURE_HP_BAR);
}

ArmorBar::ArmorBar() : Canvas(Class<ArmorBar>::ID()) {
	UIImage* pArmorBar = AddObject<UIImage>();
	pArmorBar->pLayout->Padding = { 20.f, 20.f };
	pArmorBar->pScale->LocalPosition({ 0.5f, 1.6f });
	pArmorBar->pScale->LocalSize({ 2.f, 2.f });
	pArmorBar->pTexture->Handler.Set(TEXTURE_ARMOR_BAR);
}