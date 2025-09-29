#pragma once

#include <string>
#include <Nt/Core/NtTypes.h>
#include <unordered_map>

#ifdef GetClassName
#	undef GetClassName
#endif

using ClassID = uInt;

class ClassManager {
public:
	using ClassMap = std::unordered_map<ClassID, std::string>;

public:
	[[nodiscard]] static ClassManager& Instance() noexcept;

	template <class _Ty>
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::string GetName(const ClassID& id) const;

	template <class _Ty>
	void BindName(std::string name);

private:
	ClassMap m_Classes;
};

class Identifier {
protected:
	explicit Identifier(const ClassID& id) noexcept;

public:
	Identifier() = delete;
	Identifier(const Identifier&) noexcept = default;
	Identifier(Identifier&&) noexcept = default;
	virtual ~Identifier() noexcept = default;

	Identifier& operator = (const Identifier&) noexcept = default;
	Identifier& operator = (Identifier&&) noexcept = default;

	[[nodiscard]] std::string GetClassName() const;
	[[nodiscard]] constexpr ClassID GetID() const noexcept {
		return m_ID;
	}

private:
	ClassID m_ID;
};

template <class>
struct Class {
	template <class _U>
	[[nodiscard]] static constexpr Bool Is() noexcept;
	[[nodiscard]] static constexpr Bool Is(const ClassID& otherID) noexcept;
	[[nodiscard]] static constexpr Bool Is(const Identifier& other) noexcept;
	[[nodiscard]] static constexpr ClassID ID() noexcept;
};

#include <ClassManager.inl>