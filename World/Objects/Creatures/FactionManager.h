#pragma once

#include <World/Objects/Creatures/Faction.h>

class FactionManager final {
public:
	FactionManager() noexcept = default;
	FactionManager(const FactionManager&) noexcept = default;
	FactionManager(FactionManager&&) noexcept = default;
	~FactionManager() noexcept = default;

	FactionManager& operator = (const FactionManager&) noexcept = default;
	FactionManager& operator = (FactionManager&&) noexcept = default;

	Faction& AddFaction(const std::string& npcName, std::string factionName);

	void Load(std::string fileName);

private:
	std::unordered_map<std::string, FactionPtr> m_Factions;
};