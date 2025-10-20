#pragma once

#include <World/Components/BasicComponents.h>
#include <stack>

template <typename _Ty>
class GameResource {
public:
	GameResource() noexcept = default;
	virtual ~GameResource() noexcept = default;

	[[nodiscard]] const _Ty& GetAmount() const noexcept {
		return m_Amount;
	}
	[[nodiscard]] const _Ty& GetMaxAmount() const noexcept {
		return m_MaxAmount;
	}

	void SetAmount(const _Ty& amount) noexcept {
		m_Amount = Nt::Clamp(amount, _Ty(0), m_MaxAmount);
	}
	void SetMaxAmount(const _Ty& amount) noexcept {
		m_MaxAmount = amount;
	}

protected:
	_Ty m_Amount;
	_Ty m_MaxAmount;
};

class Health final : public IComponent, public GameResource<Float> {
public:
	explicit Health(GameObject* pObject) : IComponent(pObject)
	{
	}
	~Health() noexcept override = default;

	void Drain(const Float& amount) noexcept {
		SetAmount(m_Amount - amount);
	}
	void Heal(const Float& amount) noexcept {
		SetAmount(m_Amount + amount);
	}

	void Kill() noexcept {
		m_Amount = 0.f;
	}
	void Revive() noexcept {
		m_Amount = m_MaxAmount;
	}

	[[nodiscard]] Bool IsAlive() const noexcept {
		return m_Amount > 0.f;
	}
};

class Armor final : public IComponent, public GameResource<Float> {
public:
	explicit Armor(GameObject* pObject) : IComponent(pObject)
	{
	}
	~Armor() noexcept override = default;

	void TakeDamage(const Float& amount) noexcept {
		SetAmount(m_Amount - amount);
	}
	void Repair(const Float& amount) noexcept {
		SetAmount(m_Amount + amount);
	}

	void Break() noexcept {
		m_Amount = 0.f;
	}
	void Restore() noexcept {
		m_Amount = m_MaxAmount;
	}

	[[nodiscard]] Bool IsIntact() const noexcept {
		return m_Amount > 0;
	}
};

class IBehavior;

template <typename _Ty, uInt Dimension>
class Intent final : public IComponent {
public:
	struct Target final {
		const Transform<_Ty, Dimension>* pTransform = nullptr;
		Health* pHealth = nullptr;
		Armor* pArmor = nullptr;
		const GameObject* pObject = nullptr;
	};

	struct View final {
		_Ty Range = _Ty(0);
		_Ty FOV = _Ty(0);
	};

public:
	explicit Intent(GameObject* pObject) : IComponent(pObject)
	{
	}
	~Intent() noexcept override = default;

	void UnmarkRetargeting() noexcept {
		m_HasRetargeting = true;
	}

	[[nodiscard]] IBehavior* GetBehaviorPtr() const noexcept {
		return m_pBehavior;
	}
	[[nodiscard]] const Target& GetTarget() const noexcept {
		return m_Target;
	}
	[[nodiscard]] const View& GetView() const noexcept {
		return m_View;
	}
	[[nodiscard]] uInt GetMemoryTimeMs() const noexcept {
		return m_MemoryTimeMs;
	}
	[[nodiscard]] Bool HasRetargeting() const noexcept {
		return m_HasRetargeting;
	}
	[[nodiscard]] Bool HasTarget() const noexcept {
		return m_Target.pObject != nullptr;
	}

	void SetTarget(const Target& target) noexcept {
		if (m_Target.pObject == target.pObject)
			return;

		m_Target = target;
		m_HasRetargeting = true;
	}
	void SetView(const View& view) noexcept {
		m_View = view;
	}
	void SetBehavior(IBehavior* pBehavior) noexcept {
		m_pBehavior = pBehavior;
	}
	void SetMemoryTime(const uInt& milliseconds) noexcept {
		m_MemoryTimeMs = milliseconds;
	}

private:
	uInt m_MemoryTimeMs = 0;
	Target m_Target;
	View m_View;
	IBehavior* m_pBehavior = nullptr;
	Bool m_HasRetargeting = true;
};

template <typename _Ty, uInt Dimension>
class Route final : public IComponent {
public:
	using Transform = Transform<_Ty, Dimension>;
	using Vector = typename Transform::Vector;

	struct Marker final {
		Vector Position;
		_Ty Speed = 0.f;
	};

public:
	explicit Route(GameObject* pObject) : IComponent(pObject) {
		m_pTransform = pObject->GetComponent<Transform>();
		if (m_pTransform == nullptr)
			m_pTransform = pObject->AddComponent<Transform>();
	}
	~Route() noexcept override = default;

	void PushMarker(const Marker& marker) {
		m_Markers.push(marker);
	}
	void PopMarker() {
		Assert(!m_Markers.empty(), "Out of range");
		m_Markers.pop();
	}
	Bool IsReachedMarker(const Float& deltaTime) noexcept {
		if (m_Markers.empty())
			return true;

		const Marker& marker = m_Markers.top();
		const _Ty directionSq = m_pTransform->DistanceSquare(marker.Position);
		const _Ty thresholdSq = marker.Speed * marker.Speed * deltaTime;
		return directionSq <= thresholdSq;
	}
	void Clear() {
		while (!m_Markers.empty())
			m_Markers.pop();
	}

	[[nodiscard]] Vector DirectionToMarker() {
		Assert(!m_Markers.empty(), "Out of range");
		return m_pTransform->CalculateAngle(m_Markers.top().Position);
	}

	[[nodiscard]] const Marker& GetMarker() const {
		Assert(!m_Markers.empty(), "Out of range");
		return m_Markers.top();
	}
	[[nodiscard]] Bool HasMarker() const noexcept {
		return !m_Markers.empty();
	}

public:
	_Ty InteractionDistance = _Ty(0);

private:
	std::stack<Marker> m_Markers;
	Transform* m_pTransform;
};

using Intent2D = Intent<Float, 2>;
using Route2D = Route<Float, 2>;

using Intent3D = Intent<Float, 3>;
using Route3D = Route<Float, 3>;