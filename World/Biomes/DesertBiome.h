#pragma once

#include <World/BiomeRegistrar.h>
#include <Core/StdH.h>

class DesertBiome : public Biome {
public:
	DesertBiome(const uInt& id) :
		Biome(id, {
			.Offset = { },
			.AdditionHeight = 10.0,
			.Frequency = 0.001,
			.Amplitude = 10.0,
			.Lacunarity = 2.0,
			.Gain = 0.4,
			.Min = 5.0,
			.Max = 40.0,
			.NumOctaves = 3
			})
	{
	}

	void Construct() const override {

	}

	[[nodiscard]]
	std::string GetName() const override {
		return "Desert";
	}

	[[nodiscard]]
	std::string GetTexturePath() const override {
		return PathManager::Textures() + "World\\Sand.tga";
	}

private:
	inline static BiomeRegistrar<DesertBiome> m_Registrar { "Desert" };
};
