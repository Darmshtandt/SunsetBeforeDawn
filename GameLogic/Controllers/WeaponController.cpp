#include <GameLogic/Controllers/WeaponController.h>
#include <World/Components/CombatComponents.h>
#include <World/Objects/Player.h>
#include <Core/BusLocator.h>
#include <Core/Input/Commands.h>

WeaponController::WeaponController(NotNull<Player*> pPlayer) :
	m_pWeaponBelt(RequireNotNull(pPlayer->GetComponent<WeaponBelt>())),
	m_pCombat(RequireNotNull(pPlayer->GetComponent<Combat>())),
	m_pTransform(RequireNotNull(pPlayer->GetComponent<Transform3D>())),
	m_pPlayer(pPlayer)
{
	m_pSelectedWeapon = m_pWeaponBelt->GetFirst();
	if (m_pSelectedWeapon != nullptr)
		m_pCombat->SetDamageType(m_pSelectedWeapon->GetDamageType());

	m_ActionSubscription = BusLocator::ActionDispatcher()
		.Subscribe<InputCommand>([this] (const InputCommand& input) {
		if (input.ContextID == InputContextID::Combat)
			Execute(input.Commands);
			});
}

WeaponController::~WeaponController() noexcept {
	BusLocator::ActionDispatcher().Unsubscribe<InputCommand>(m_ActionSubscription);
}

void WeaponController::Update(const Float& deltaTime) {
	(void)deltaTime;
}

void WeaponController::Execute(const std::unordered_set<InputCommandID>& commands) {
	if (m_pSelectedWeapon == nullptr)
		return;

	if (commands.contains(InputCommandID::Fire))
		m_pCombat->PerformAttack(m_pTransform->Position(), m_pPlayer->GetFullRotation());
}