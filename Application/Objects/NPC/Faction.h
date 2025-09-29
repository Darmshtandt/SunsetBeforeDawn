#pragma once

#include <Nt/Core/NtTypes.h>
#include <memory>
#include <string>
#include <unordered_map>

class Faction final {
public:
	enum class Relation : Byte {
		Neutral,
		Friendy,
		Hostile
	};

	using RelationMap = std::unordered_map<const Faction*, Relation>;

public:
	explicit Faction(std::string name) noexcept;

	Faction() = delete;
	Faction(const Faction&) noexcept = default;
	Faction(Faction&&) noexcept = default;
	~Faction() noexcept = default;

	Faction& operator=(const Faction&) noexcept = default;
	Faction& operator=(Faction&&) noexcept = default;

	[[nodiscard]] Relation GetRelation(const Faction* other) const;

	void SetRelation(const Faction* other, Relation relation);

private:
	RelationMap m_Relations;
	std::string m_Name;
};

using FactionPtr = std::unique_ptr<Faction>;