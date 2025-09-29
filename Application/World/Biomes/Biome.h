#pragma once

#include <World/Terrain.h>
#include <RegistrarBase.h>

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

//using BiomePtr = std::unique_ptr<Biome>;
//using BiomeCreator = BiomePtr(*)(const uInt& id);
//
//class BiomeFactory : public FactoryBase<BiomePtr, BiomeCreator> {
//public:
//	[[nodiscard]]
//	Type Create(const std::string& name, const uInt& id) {
//		if (m_Objects.find(name) == m_Objects.end())
//			Raise("Not found:" + name);
//
//		return m_Objects[name](id);
//	}
//};
//
//template <class _Ty>
//class BiomeRegistrar {
//public:
//	BiomeRegistrar(const std::string& name) {
//		BiomeFactory::Instance().Register(name, [](const uInt& id) -> ObjectPtr {
//			return std::make_unique<_Ty>(id);
//			});
//	}
//	BiomeRegistrar(const BiomeRegistrar&) = delete;
//	BiomeRegistrar(BiomeRegistrar&&) = delete;
//};