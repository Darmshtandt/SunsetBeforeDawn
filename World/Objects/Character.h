#pragma once

#include <World/Objects/GameObject.h>

class Collider;
class MeshRenderer;
class TextureRenderer;
class Health;
class Armor;

class Character : public GameObject {
protected:
	Character(const ClassID& id, const ObjectType& type, std::string factionName);

public:
	Character() = delete;
	~Character() noexcept override = default;

	[[nodiscard]] std::string GetFactionName() const noexcept;

protected:
	Transform3D* m_pTransform;
	Movement3D* m_pMovement;
	Collider* m_pCollider;

	MeshRenderer* m_pMesh;
	TextureRenderer* m_pTexture;
	Health* m_pHealth;
	Armor* m_pArmor;

private:
	std::string m_FactionName;
};