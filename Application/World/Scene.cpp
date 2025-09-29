// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Scene.h>
#include <Helpers/Profiler.h>

void Scene::Initialize(Nt::Shader* pShader) {
	m_pShader = RequireNotNull(pShader);

	Profiler::Instance().Start();
	std::shared_ptr<Landscape> pLandscape = std::make_shared<Landscape>();
	m_Statics.emplace_back(pLandscape);

	m_pLandscape = pLandscape;
	m_pLandscape.lock()->Create({ 16384.0, 16384.0 }, { 2, 2 });
	m_pShader->SetUniform("TerrainSize", m_pLandscape.lock()->GetSize());
	Profiler::Instance().Stop("Create Terrain time");
}

void Scene::Generate() {
	if (!m_pLandscape.expired())
		m_pLandscape.lock()->Generate(256);
}

void Scene::Clear() {
	EventBus::Instance().Emmit<OnClear>({ });

	m_Players.clear();
	m_Creatures.clear();
	m_Interactives.clear();
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
	for (const CreaturePtr& pObject : m_Creatures)
		pObject->Update(deltaTime);
	for (const InteractivePtr& pObject : m_Interactives)
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

void Scene::Render(NotNull<Nt::Renderer*> pRenderer) const {
	for (const PlayerPtr& pObject : m_Players)
		pObject->Render(pRenderer);
	for (const CreaturePtr& pObject : m_Creatures)
		pObject->Render(pRenderer);
	for (const InteractivePtr& pObject : m_Interactives)
		pObject->Render(pRenderer);
	for (const ObjectPtr& pObject : m_Statics) {
		if (Class<Landscape>::Is(*pObject))
			m_pShader->SetUniform("IsTerrain", true);

		pObject->Render(pRenderer);

		if (Class<Landscape>::Is(*pObject))
			m_pShader->SetUniform("IsTerrain", false);
	}
}

const std::vector<PlayerPtr>& Scene::GetAllPlayers() const noexcept {
	return m_Players;
}
const std::vector<CreaturePtr>& Scene::GetAllCreatures() const noexcept {
	return m_Creatures;
}
const std::vector<InteractivePtr>& Scene::GetAllInteractives() const noexcept {
	return m_Interactives;
}
const std::vector<ObjectPtr>& Scene::GetAllStatics() const noexcept {
	return m_Statics;
}