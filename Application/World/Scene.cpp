// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Actions/IAction.h>
#include <World/Scene.h>
#include <Helpers/Profiler.h>
#include <World/Landscape.h>

void Scene::Initialize() {
	Profiler::Instance().Start();
	std::shared_ptr<Landscape> pLandscape = std::make_shared<Landscape>();
	m_Statics.emplace_back(pLandscape);

	m_pLandscape = pLandscape;
	m_pLandscape.lock()->Create({ 16384.0, 16384.0 }, { 2, 2 });
	Profiler::Instance().Stop("Create Terrain time");

	m_Dispatcher.Emmit<OnAdd>({ pLandscape.get() });
}

void Scene::Generate() {
	if (!m_pLandscape.expired())
		m_pLandscape.lock()->Generate(256);
}

void Scene::Clear() {
	m_Dispatcher.Emmit<OnClear>({ });

	m_Dynamic.clear();
	m_Statics.clear();
}

void Scene::AddAction(const NotNull<ActionPtr>& pAction) {
	m_ActionList.push_back(pAction);
}
void Scene::RemoveAction(const NotNull<ActionPtr>& pAction) {
	std::erase_if(m_ActionList, [pAction] (const ActionPtr& pOther) {
		return pAction.Get() == pOther;
		});
}

void Scene::Update(const Float& deltaTime) {
	for (const ObjectPtr& pObject : m_Dynamic)
		pObject->Update(deltaTime);

	for (auto it = m_ActionList.begin(); it != m_ActionList.end();) {
		IAction* pAction = it->get();

		if (pAction->IsDone()) {
			it = m_ActionList.erase(it);
			break;
		}

		pAction->Execute(deltaTime);
		++it;
	}
}

EventBus* Scene::GetDispatcherPtr() noexcept {
	return &m_Dispatcher;
}

const std::vector<ObjectPtr>& Scene::GetDynamicObjects() const noexcept {
	return m_Dynamic;
}
const std::vector<ObjectPtr>& Scene::GetStaticObjects() const noexcept {
	return m_Statics;
}