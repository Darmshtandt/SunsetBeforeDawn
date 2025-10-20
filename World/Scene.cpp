// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <World/Scene.h>
#include <Core/Helpers/Profiler.h>
#include <World/Landscape.h>

void Scene::Initialize() {
	Profiler::Instance().Start();
	std::shared_ptr<Landscape> pLandscape = std::make_shared<Landscape>();
	m_Objects.emplace_back(pLandscape);

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
	m_Objects.clear();
}

void Scene::Update(const Float& deltaTime) {
	for (const Handler* pHandler : m_Handlers)
		pHandler->Function(deltaTime);
}

EventBus* Scene::GetDispatcherPtr() noexcept {
	return &m_Dispatcher;
}

const std::vector<ObjectPtr>& Scene::GetObjects() const noexcept {
	return m_Objects;
}