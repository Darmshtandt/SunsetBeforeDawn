// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Scene.h>
#include <Core/Helpers/Profiler.h>
#include <World/Landscape.h>

Scene::Scene(NotNull<EventBus*> pBus) noexcept :
	m_pDispatcher(pBus)
{
}

void Scene::Initialize() {
	Profiler::Instance().Start();
	std::shared_ptr<Landscape> pLandscape = std::make_shared<Landscape>();
	//m_Objects.emplace_back(pLandscape);

	m_pLandscape = pLandscape;
	m_pLandscape.lock()->Create({ 16384.0, 16384.0 }, { 2, 2 });
	Profiler::Instance().Stop("Create Terrain time");

	//m_pDispatcher->Emmit<OnAdd>({ pLandscape.get() });
}

void Scene::Generate() {
	if (!m_pLandscape.expired())
		m_pLandscape.lock()->Generate(256);
}

void Scene::AddObject(const Nt::Float3D& position, ClassID id) {
	ObjectPtr pObject = ObjectFactory::Instance().Create(id);

	if (position.LengthSquare() != 0.f) {
		auto* pTransform =
			RequireNotNull(pObject->GetComponent<Transform3D>());
		pTransform->LocalPosition(position);
	}

	const auto* pHandler = pObject->GetComponent<Handler>();
	if (pHandler != nullptr && pHandler->Function)
		m_Handlers.emplace_back(pHandler);

	m_pDispatcher->Emmit<OnAdd>({ pObject.get() });
	m_Objects.emplace_back(std::move(pObject));
}

void Scene::RemoveObject(GameObject& object) {
	const auto it = 
		std::find_if(m_Objects.cbegin(), m_Objects.cend(), [&object](const ObjectPtr& pObject) {
			return pObject.get() == &object;
		});
	if (it == m_Objects.cend()) {
		Raise("Unknown object: " + object.GetClassName());
		return;
	}

	m_pDispatcher->Emmit<OnRemove>({ &object });

	const auto* pHandler = object.GetComponent<Handler>();
	if (pHandler != nullptr && pHandler->Function)
		std::erase(m_Handlers, pHandler);

	m_Objects.erase(it);
}

void Scene::Clear() {
	m_pDispatcher->Emmit<OnClear>({ });
	m_Objects.clear();
}

void Scene::Update(const Float& deltaTime) {
	for (const Handler* pHandler : m_Handlers)
		pHandler->Function(deltaTime);

	for (uInt i = 0; i < m_Objects.size();) {
		GameObject* pObject = m_Objects[i].get();
		if (!pObject->IsValid())
			RemoveObject(*pObject);
		else
			++i;
	}
}

const std::vector<ObjectPtr>& Scene::GetObjects() const noexcept {
	return m_Objects;
}