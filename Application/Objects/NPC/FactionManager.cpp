#include <Objects/NPC/FactionManager.h>
#include <Nt/Core/Utilities.h>

Faction& FactionManager::AddFaction(const std::string& npcName, std::string factionName) {
	FactionPtr pFaction = std::make_unique<Faction>(std::move(factionName));
	Faction* pFactionRaw = pFaction.get();

	m_Factions[npcName] = std::move(pFaction);

	return *pFactionRaw;
}

void FactionManager::Load(std::string fileName) {

}
