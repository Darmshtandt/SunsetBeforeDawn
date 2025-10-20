#pragma once

#include <World/Objects/Character.h>
#include <World/Components/GameComponents.h>

class MeshRenderer;
class TextureRenderer;

class Creature : public Character {
protected:
	Creature(const ClassID& id, std::string factionName);

public:
	Creature() = delete;
	Creature(const Creature&) noexcept = default;
	Creature(Creature&&) noexcept = default;
	~Creature() noexcept override = default;

	Creature& operator = (const Creature&) noexcept = default;
	Creature& operator = (Creature&&) noexcept = default;

protected:
	Transform3D* m_pTransform = nullptr;
	Movement3D* m_pMovement = nullptr;
	MeshRenderer* m_pMesh = nullptr;
	TextureRenderer* m_pTexture = nullptr;
	Route3D* m_pRoute;
	Intent3D* m_pIntent;
};

using CreaturePtr = std::shared_ptr<Creature>;