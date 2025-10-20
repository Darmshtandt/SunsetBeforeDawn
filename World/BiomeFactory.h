#pragma once

#include <World/Biome.h>
#include <unordered_map>

class BiomeFactory {
public:
	using BiomeCreator = std::unique_ptr<Biome>(*)(const uInt& id);

	[[nodiscard]]
	static BiomeFactory& Instance() noexcept {
		static BiomeFactory instance;
		return instance;
	}

	void RegisterBiome(const std::string& name, BiomeCreator creator) {
		if (m_Biomes.contains(name))
			Raise("A biome with this name is already registered");

		m_Biomes[name] = creator;
	}

	std::unique_ptr<Biome> CreateBiome(const std::string& name, const uInt& id) {
		if (m_Biomes.find(name) == m_Biomes.end())
			Raise("Biome \"" + name + "\" not found");

		return m_Biomes[name](id);
	}

	std::vector<std::string> GetBiomeNames() const {
		std::vector<std::string> names;
		for (const std::pair<std::string, BiomeCreator>& biome : m_Biomes)
			names.push_back(biome.first);

		return names;
	}

private:
	std::unordered_map<std::string, BiomeCreator> m_Biomes;
};