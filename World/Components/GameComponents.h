#pragma once

#include <Engine/AI/AIContext.h>
#include <World/Components/BasicComponents.h>
#include <stack>
#include <variant>
#include <Nt/Core/Timer.h>

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
	explicit Health(GameObject* pObject) :
		IComponent(pObject, Class<Health>::ID())
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
	explicit Armor(GameObject* pObject) :
		IComponent(pObject, Class<Armor>::ID())
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

template <typename _Ty, uInt Dimension>
class Intent final : public IComponent {
public:
	struct View final {
		_Ty Range = _Ty(0);
		_Ty FOV = _Ty(0);
	};

public:
	explicit Intent(GameObject* pObject) :
		IComponent(pObject, Class<Intent>::ID())
	{
	}
	~Intent() noexcept override = default;

	void UnmarkRetargeting() noexcept {
		m_HasRetargeting = true;
	}

	[[nodiscard]] const LivingPawn& GetTarget() const noexcept {
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

	void SetTarget(const LivingPawn& target) noexcept {
		if (m_Target.pObject == target.pObject)
			return;

		m_Target = target;
		m_HasRetargeting = true;
	}
	void SetView(const View& view) noexcept {
		m_View = view;
	}
	void SetMemoryTime(const uInt& milliseconds) noexcept {
		m_MemoryTimeMs = milliseconds;
	}

private:
	uInt m_MemoryTimeMs = 0;
	LivingPawn m_Target;
	View m_View;
	Bool m_HasRetargeting = true;
};

template <typename _Ty, uInt Dimension>
class Route final : public IComponent {
public:
	using Transform = TransformSelector<_Ty, Dimension>::Type;
	using Vector = typename Transform::Vector;

	struct Marker final {
		Vector Position;
		_Ty Speed = 0.f;
	};

public:
	explicit Route(GameObject* pObject) :
		IComponent(pObject, Class<Route>::ID())
	{
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
	Bool IsReachedMarker() noexcept {
		if (m_Markers.empty())
			return true;

		const Marker& marker = m_Markers.top();
		const _Ty directionSq = m_pTransform->DistanceSquare(marker.Position);
		return directionSq <= InteractionDistance * InteractionDistance;
	}
	Bool IsReachedMarker(const Float& deltaTime) noexcept {
		if (m_Markers.empty())
			return true;

		const Marker& marker = m_Markers.top();
		const _Ty directionSq = m_pTransform->DistanceSquare(marker.Position);
		const _Ty thresholdSq = marker.Speed * marker.Speed * deltaTime;
		return directionSq <= thresholdSq + InteractionDistance * InteractionDistance;
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
	[[nodiscard]] uInt GetMarkerCount() const noexcept {
		return m_Markers.size();
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

class Blackboard final : public IComponent {
public:
	using Value = std::variant<
		Int, uInt, Bool, Float, std::string,
		Nt::Float2D, Nt::Float3D, Nt::Float4D,
		Nt::Timer, LivingPawn>;

public:
	explicit Blackboard(GameObject* pObject) :
		IComponent(pObject, Class<Blackboard>::ID())
	{
	}
	~Blackboard() noexcept override = default;


	[[nodiscard]]
	Bool Contains(const std::string& name) const noexcept {
		return m_Map.contains(name);
	}
	void Erase(const std::string& name) noexcept {
		m_Map.erase(name);
	}
	void Clear() noexcept {
		m_Map.clear();
	}

	template <class _Ty>
	void Set(const std::string& name, _Ty&& value) {
		m_Map[name] = std::forward<_Ty>(value);
	}

	template <class _Ty>
	std::optional<_Ty> Get(const std::string& name) noexcept {
		auto it = m_Map.find(name);
		if (it == m_Map.cend())
			return std::nullopt;

		std::add_pointer_t<_Ty> pValue = std::get_if<_Ty>(&it->second);
		if (pValue == nullptr)
			return std::nullopt;

		return *pValue;
	}

private:
	std::unordered_map<std::string, Value> m_Map;
};


using Intent2D = Intent<Float, 2>;
using Route2D = Route<Float, 2>;

using Intent3D = Intent<Float, 3>;
using Route3D = Route<Float, 3>;