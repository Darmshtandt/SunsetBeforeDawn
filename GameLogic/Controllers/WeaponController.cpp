#include <GameLogic/Controllers/WeaponController.h>
#include <World/Components/CombatComponents.h>

WeaponController::WeaponController(const GameObject& object) :
	m_pWeaponBelt(RequireNotNull(object.GetComponent<WeaponBelt>()))
{
	m_pSelectedWeapon = m_pWeaponBelt->GetFirst();
}

void WeaponController::Update(const Float& deltaTime) {
	(void)deltaTime;
}

void WeaponController::Execute(const std::unordered_set<InputCommandID>& commands) {
	if (m_pSelectedWeapon == nullptr)
		return;

	//if (commands.contains(Command::Fire))

}