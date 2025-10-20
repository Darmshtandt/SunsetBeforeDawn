#pragma once

#include <World/Terrain.h>

class Biome {
public:
	using Property = Terrain::Config::Property;

public:
	Biome(const uInt& id, const Property& property) : 
		m_Property(property),
		m_ID(id)
	{
	}

	virtual void Construct() const = 0;

	void AddVertex(NotNull<Nt::Vertex*> pVertex) noexcept {
		std::lock_guard lock(m_Mutex);
		m_Vertices.push_back(pVertex);
	}
	
	[[nodiscard]]
	const uInt& GetID() const noexcept {
		return m_ID;
	}

	[[nodiscard]]
	virtual std::string GetName() const = 0;

	[[nodiscard]]
	virtual std::string GetTexturePath() const = 0;

	[[nodiscard]]
	const Property& GetProperty() const noexcept {
		return m_Property;
	}

private:
	const Property m_Property;
	std::vector<Nt::Vertex*> m_Vertices;
	std::mutex m_Mutex;
	uInt m_ID;
};